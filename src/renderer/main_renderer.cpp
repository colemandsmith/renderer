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

static const char *vShader = "Shaders/shader.vert";
static const char *fShader = "Shaders/shader.frag";

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

void CreateSimplePolygons() {
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
}

void SetupObjects() {
  mainWindow = Window(1366, 768);
  mainWindow.Initialize();
  CreateSimplePolygons();

  brickTexture = Texture("Textures/brick.png");
  brickTexture.LoadTextureA();

  dirtTexture = Texture("Textures/dirt.png");
  dirtTexture.LoadTextureA();

  shinyMaterial = Material(4.0f, 256);
  dullMaterial = Material(0.3f, 0);

  mainLight = DirectionalLight(2048, 2048, 1.0f, 0.53f, 0.3f, 0.1f, 0.8f,
                               -10.0f, -12.0f, 19.0f);
}

void RenderScene(Shader *shader) {
  glm::mat4 model(1.0f);

  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  brickTexture.UseTexture();
  shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

  // meshList[0]->RenderMesh();

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  dirtTexture.UseTexture();
  dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

  // meshList[1]->RenderMesh();

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
  glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
  dirtTexture.UseTexture();
  shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

  // meshList[2]->RenderMesh();
}

int main() {

  SetupObjects();
  glm::mat4 projection =
      glm::perspective(glm::radians(60.0f),
                       (GLfloat)mainWindow.getBufferWidth() /
                           (GLfloat)mainWindow.getBufferHeight(),
                       0.1f, 100.0f);
  OpenGLRenderer *renderer = new OpenGLRenderer();

  // Loop until window closed
  while (!mainWindow.getShouldClose()) {
    GLfloat now = glfwGetTime();
    deltaTime = now - lastTime;
    lastTime = now;

    // Get and handle user input events
    glfwPollEvents();

    camera.KeyControl(mainWindow.getKeys(), deltaTime);
    camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

    // if (mainWindow.getKeys()[GLFW_KEY_L]) {
    //     spotLights[0].Toggle();
    //     mainWindow.getKeys()[GLFW_KEY_L] = false;
    // }

    // PerformRenderPasses(projection);

    mainWindow.SwapBufffers();
  }

  return 0;
}
