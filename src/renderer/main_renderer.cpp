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

#include "DirectionalLight.h"
#include "Mesh.h"
#include "OpenGLRenderer.h"
#include "Shader.h"
#include "Window.h"

Window mainWindow;
Camera camera;

std::vector<Mesh *> meshList;
std::vector<Shader *> shaderList;
Shader directionalShadowShader;

Texture brickTexture;
Texture dirtTexture;
Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

float deltaTime = 0.0f;
float lastTime = 0.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
       uniformEyePosition = 0, uniformSpecularIntensity = 0,
       uniformShininess = 0, uniformOmniLightPos, uniformFarPlane;

static const char *vShader = "Shaders/unlit.vert";
static const char *fShader = "Shaders/unlit.frag";

void CreateShaders() {
  Shader *defaultShader = new Shader();
  defaultShader->CreateFromFiles(vShader, fShader);
  shaderList.push_back(defaultShader);

  Shader *normalMapShader = new Shader();
  normalMapShader->CreateFromFiles("Shaders/normal_map_shader.vert",
                                   "Shaders/normal_map_shader.frag");
  shaderList.push_back(normalMapShader);

  directionalShadowShader = Shader();
  directionalShadowShader.CreateFromFiles(
      "Shaders/directional_shadow_map.vert",
      "Shaders/directional_shadow_map.frag");

  // omniShadowShader.CreateFromFiles(
  //     "Shaders/omni_shadow_map.vert",
  //     "Shaders/omni_shadow_map.geom",
  //     "Shaders/omni_shadow_map.frag"
  // );
}

void CalcAverageNormals(unsigned int *indices, unsigned int indexCount,
                        GLfloat *vertices, unsigned int vertexCount,
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

void CreateSimplePolygons(OpenGLRenderer *renderer) {
  unsigned int indices[] = {
      2, 1, 0, 0, 1, 3, 3, 1, 2, 2, 0, 3,
  };
  GLfloat vertices[] = {
      //   x       y     z         u     v       nx    ny    nz      tx    ty tz
      -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f,  -1.0f, 1.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f,  1.0f,  0.0f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  };

  CalcAverageNormals(indices, 12, vertices, 44, 11, 5);

  unsigned int floorIndices[] = {
      0, 2, 1, 1, 2, 3,
  };

  GLfloat floorVertices[] = {
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

void SetupObjects(OpenGLRenderer *renderer) {
  mainWindow = Window(1366, 768);
  mainWindow.Initialize();
  CreateShaders();
  CreateSimplePolygons(renderer);

  brickTexture = Texture("Textures/brick.pnt");
  renderer->SubmitTexture(&brickTexture);

  dirtTexture = Texture("Textures/dirt.png");
  renderer->SubmitTexture(&dirtTexture);

  shinyMaterial = Material(4.0f, 256);
  dullMaterial = Material(0.3f, 0);

  mainLight = DirectionalLight(2048, 2048, 1.0f, 0.53f, 0.3f, 0.1f, 0.8f,
                               -10.0f, -12.0f, 19.0f);
}

void RenderScene(OpenGLRenderer *renderer) {
  glm::mat4 model(1.0f);

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  renderer->UseTexture(&brickTexture);
  shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

  renderer->RenderMesh(meshList[0]);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  renderer->UseTexture(&dirtTexture);
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

  renderer->RenderMesh(meshList[1]);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  renderer->UseTexture(&dirtTexture);
  shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

  renderer->RenderMesh(meshList[2]);

}

void MainRenderPass(OpenGLRenderer *renderer, glm::mat4 projection,
                    glm::mat4 view) {
  glViewport(0, 0, 1366, 768);

  // clear window
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  shaderList[0]->UseShader();

  uniformModel = shaderList[0]->GetModelLocation();
  uniformProjection = shaderList[0]->GetProjectionLocation();
  uniformView = shaderList[0]->GetViewLocation();
  uniformEyePosition = shaderList[0]->GetEyePositionLocation();
  uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
  uniformShininess = shaderList[0]->GetShininessLocation();

  glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                     glm::value_ptr(projection));
  glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
  glUniform3f(uniformEyePosition, camera.GetCameraPosition().x,
              camera.GetCameraPosition().y, camera.GetCameraPosition().z);

  shaderList[0]->SetDirectionalLight(&mainLight);

  RenderScene(renderer);
}

void PerformRenderPasses(OpenGLRenderer *renderer, Camera &camera,
                 glm::mat4 projection) {
  MainRenderPass(renderer, projection, camera.CalculateViewMatrix());
}

int main() {
  OpenGLRenderer *renderer = new OpenGLRenderer();
  SetupObjects(renderer);
  glm::mat4 projection =
      glm::perspective(glm::radians(60.0f),
                       (GLfloat)mainWindow.getBufferWidth() /
                           (GLfloat)mainWindow.getBufferHeight(),
                       0.1f, 100.0f);

  // Loop until window closed
  while (!mainWindow.getShouldClose()) {
    GLfloat now = glfwGetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    // Get and handle user input events
    glfwPollEvents();

    camera.KeyControl(mainWindow.getKeys(), deltaTime);
    camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

    if (mainWindow.getKeys()[GLFW_KEY_L]) {
      spotLights[0].Toggle();
      mainWindow.getKeys()[GLFW_KEY_L] = false;
    }

   PerformRenderPasses(renderer, camera, projection);

    mainWindow.SwapBufffers();
  }

  return 0;
}
