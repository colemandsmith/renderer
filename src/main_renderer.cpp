#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "resources/DirectionalLight.h"
#include "resources/Mesh.h"
#include "rendering/RenderEngine.h"
#include "rendering/Shader.h"

Camera camera;

std::vector<Mesh *> meshList;
Shader *defaultShader;

Texture *brickTexture;
Texture *dirtTexture;

DirectionalLight mainLight;
// PointLight pointLights[MAX_POINT_LIGHTS];
// SpotLight spotLights[MAX_SPOT_LIGHTS];

float deltaTime = 0.0f;
float lastTime = 0.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
       uniformEyePosition = 0, uniformSpecularIntensity = 0,
       uniformShininess = 0, uniformOmniLightPos, uniformFarPlane;

static const char *vShader = "shaders/shader.vert";
static const char *fShader = "shaders/shader.frag";

ShaderId CreateShaders(ShaderManager* shaderManager) {
  return shaderManager->AddShaderFromFiles(vShader, fShader);
}

void CalcAverageNormals(unsigned int *indices, unsigned int indexCount,
                        float *vertices, unsigned int vertexCount,
                        unsigned int vertexLength, unsigned int normalOffset) {
  for (size_t i = 0; i < indexCount; i += 3) {
    unsigned int index0 = indices[i] * vertexLength;
    unsigned int index1 = indices[i + 1] * vertexLength;
    unsigned int index2 = indices[i + 2] * vertexLength;
    glm::vec3 v1(vertices[index0] - vertices[index1],
                 vertices[index0 + 1] - vertices[index1 + 1],
                 vertices[index0 + 2] - vertices[index1 + 2]);
    glm::vec3 v2(vertices[index0] - vertices[index2],
                 vertices[index0 + 1] - vertices[index2 + 1],
                 vertices[index0 + 2] - vertices[index2 + 2]);
    glm::vec3 normal = glm::normalize(glm::cross(v2, v1));

    index0 += normalOffset;
    index1 += normalOffset;
    index2 += normalOffset;

    vertices[index0] += normal.x;
    vertices[index0 + 1] += normal.y;
    vertices[index0 + 2] += normal.z;

    vertices[index1] += normal.x;
    vertices[index1 + 1] += normal.y;
    vertices[index1 + 2] += normal.z;

    vertices[index2] += normal.x;
    vertices[index2 + 1] += normal.y;
    vertices[index2 + 2] += normal.z;
  }

  for (size_t i = 0; i < vertexCount / vertexLength; i++) {
    unsigned int nOffset = i * vertexLength + normalOffset;
    glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1],
                  vertices[nOffset + 2]);
    vec = glm::normalize(vec);
    vertices[nOffset] = vec.x;
    vertices[nOffset + 1] = vec.y;
    vertices[nOffset + 2] = vec.z;
  }
}

void CreateSimplePolygons(Renderer *renderer) {
  unsigned int indices[] = {
      2, 1, 0, 0, 1, 3, 3, 1, 2, 2, 0, 3,
  };
  float vertices[] = {
      // x      y     z      u     v    nx    ny    nz    tx    ty    tz
      -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f,  -1.0f, 1.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f,  1.0f,  0.0f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  };

  CalcAverageNormals(indices, 12, vertices, 44, 11, 5);

  unsigned int floorIndices[] = {
      0, 2, 1, 1, 2, 3,
  };

  float floorVertices[] = {
      -10.0f, 0.0f, -10.f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      10.0f,  0.0f, -10.f, 10.0f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      -10.f,  0.0f, 10.0f, 0.0f,  10.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      10.0f,  0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  };

  Mesh *obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, 44, 12);
  meshList.push_back(obj1);

  Mesh *obj2 = new Mesh();
  obj2->CreateMesh(vertices, indices, 44, 12);
  meshList.push_back(obj2);

  Mesh *obj3 = new Mesh();
  obj3->CreateMesh(floorVertices, floorIndices, 44, 6);
  meshList.push_back(obj3);

  for (const Mesh *mesh : meshList) {
    renderer->SubmitMesh(mesh);
  }
}

void SetupObjects(Renderer *renderer, ShaderId shaderId) {
  Material* shinyMaterial = new Material(shaderId, 4.0f, 256.0f);
  Material* dullMaterial = new Material(shaderId, 0.3f, 0.0f);

  CreateSimplePolygons(renderer);

  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                  -90.0f, 0.0f, 5.0f, 0.5f);

  brickTexture = new Texture("Textures/brick.png");
  dirtTexture = new Texture("Textures/dirt.png");
  renderer->SubmitTexture(brickTexture);
  renderer->SubmitTexture(dirtTexture);

  mainLight = DirectionalLight(2048, 2048, 1.0f, 0.53f, 0.3f, 0.1f, 0.8f,
                               -10.0f, -12.0f, 19.0f);
}

void RenderScene(Renderer *renderer) {
  glm::mat4 model(1.0f);

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  renderer->UseTexture(dirtTexture);
  renderer->UseMaterial(shinyMaterial);

  renderer->RenderMesh(meshList[0]);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  renderer->UseTexture(dirtTexture);
  renderer->UseMaterial(shinyMaterial);

  renderer->RenderMesh(meshList[1]);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  renderer->UseTexture(dirtTexture);
  renderer->UseMaterial(shinyMaterial);

  renderer->RenderMesh(meshList[2]);
}

void MainRenderPass(Renderer *renderer, glm::mat4 projection,
                    glm::mat4 view) {
  glViewport(0, 0, 1366, 768);

  // clear window
  glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  uniformModel = defaultShader->GetModelLocation();
  uniformProjection = defaultShader->GetProjectionLocation();
  uniformView = defaultShader->GetViewLocation();
  uniformEyePosition = defaultShader->GetEyePositionLocation();
  uniformSpecularIntensity = defaultShader->GetSpecularIntensityLocation();
  uniformShininess = defaultShader->GetShininessLocation();

  glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                     glm::value_ptr(projection));
  glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
  glUniform3f(uniformEyePosition, camera.GetCameraPosition().x,
              camera.GetCameraPosition().y, camera.GetCameraPosition().z);

  defaultShader->SetDirectionalLight(&mainLight);
  glm::mat4 lightTransform = mainLight.CalculateLightTransform();
  defaultShader->SetDirectionalLightTransform(&lightTransform);

  defaultShader->Validate();

  RenderScene(renderer);
}

void PerformRenderPasses(Renderer *renderer, glm::mat4 projection) {
  glm::mat4 view = camera.CalculateViewMatrix();
  MainRenderPass(renderer, projection, view);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char *message, const void *userParam) {
  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    return;

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " << message << std::endl;

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    std::cout << "Source: API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    std::cout << "Source: Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    std::cout << "Source: Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    std::cout << "Source: Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    std::cout << "Source: Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    std::cout << "Source: Other";
    break;
  }
  std::cout << std::endl;

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    std::cout << "Type: Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::cout << "Type: Deprecated Behaviour";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::cout << "Type: Undefined Behaviour";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    std::cout << "Type: Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    std::cout << "Type: Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    std::cout << "Type: Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    std::cout << "Type: Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    std::cout << "Type: Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    std::cout << "Type: Other";
    break;
  }
  std::cout << std::endl;

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    std::cout << "Severity: high";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    std::cout << "Severity: medium";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    std::cout << "Severity: low";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    std::cout << "Severity: notification";
    break;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

int main() {
  RenderEngine engine;
  ShaderId defaultShaderId = CreateShaders(engine.GetShaderManager());
  SetupObjects(engine.GetRenderer(), defaultShaderId);

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    // initialize debug output
    printf("debug init\n");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }

  // Loop until window closed
  while (!engine.GetWindow()->getShouldClose()) {
    float now = glfwGetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    // Get and handle user input events
    engine.GetWindow()->PollEvents();

    camera.KeyControl(engine.GetWindow()->getKeys(), deltaTime);
    camera.mouseControl(engine.GetWindow()->getXChange(), engine.GetWindow()->getYChange());

    engine.Render();
    // PerformRenderPasses(engine.GetRenderer(), projection);

    engine.GetWindow()->SwapBufffers();
  }

  return 0;
}
