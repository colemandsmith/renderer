#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "RenderObject.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"

Window mainWindow;
std::vector<Mesh*> meshList;
Camera camera;


std::vector<Shader*> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

std::vector<Model*> modelList;

Model brickModel;
Model orangeCat;
RenderObject orangeCatRenderObj;
Model skull;
Model donut;

Skybox skybox;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Material shinyMaterial;
Material dullMaterial;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

float catAngle;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
       uniformSpecularIntensity = 0, uniformShininess = 0,
       uniformOmniLightPos, uniformFarPlane;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

void SetupScene() {

}

void CreateShaders() {
    Shader* defaultShader = new Shader();
    defaultShader->CreateFromFiles(vShader, fShader);
    shaderList.push_back(defaultShader);

    Shader* normalMapShader = new Shader();
    normalMapShader->CreateFromFiles("Shaders/normal_map_shader.vert", "Shaders/normal_map_shader.frag");
    shaderList.push_back(normalMapShader);

    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

    omniShadowShader.CreateFromFiles(
        "Shaders/omni_shadow_map.vert",
        "Shaders/omni_shadow_map.geom",
        "Shaders/omni_shadow_map.frag"
    );
}

void CalcAverageNormals(unsigned int* indices, unsigned int indexCount, 
                        GLfloat* vertices, unsigned int vertexCount, unsigned int vertexLength,
                        unsigned int normalOffset) {
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
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; 
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}


void CreateSimplePolygons() {
    unsigned int indices[] = {
        2, 1, 0,
        0, 1, 3,
        3, 1, 2,
        2, 0, 3,
    };
    GLfloat vertices[] = {
    //   x       y     z         u     v       nx    ny    nz      tx    ty   tz 
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,      0.5f, 0.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f,     1.0f, 0.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f,    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
    };

    CalcAverageNormals(indices, 12, vertices, 44, 11, 5);

    unsigned int floorIndices[] = {
       0, 2, 1,
       1, 2, 3,
    };

    GLfloat floorVertices[] = { 
        -10.0f, 0.0f, -10.f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        10.0f, 0.0f, -10.f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        -10.f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 10.0f,    10.0f, 10.0f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 44, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 44, 12);
    meshList.push_back(obj2);

    Mesh* obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 44, 6);
    meshList.push_back(obj3);
}

void SetupObjects() {
    mainWindow = Window(1366, 768);
    mainWindow.Initialize();

    CreateSimplePolygons();
    CreateShaders();

    camera = Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f,
        5.0f,
        0.5f
    );

    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTextureA();

    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTextureA();

    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTextureA();

    shinyMaterial = Material(4.0f, 256);
    dullMaterial = Material(0.3f, 0);

    brickModel = Model();
    brickModel.LoadModelWithNormalMap("Models/brick01a.obj");

    skull = Model();
    skull.LoadModel("Models/SkullV.obj");

    orangeCat = Model();
    orangeCat.LoadModel("Models/orange_cat.obj");

    /*model = glm::rotate(model, glm::radians(catAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, -1.0f));
    model = glm::rotate(model, -glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));*/
    orangeCatRenderObj = RenderObject(
        &orangeCat, "orange cat",
        glm::vec3(-3.0f, 1.0f, -1.0f),
        glm::vec3(0.05f, 0.05f, 0.05f),
        -90.0f, 0.0f, 0.0f);

    donut = Model();
    donut.LoadModel("Models/donut.obj");

    mainLight = DirectionalLight(
        2048, 2048,
        1.0f, 0.53f, 0.3f,
        0.1f, 0.8f,
        -10.0f, -12.0f, 19.0f
    );

    pointLightCount = 0;

    pointLights[0] = PointLight(
        1024, 1024, 0.1f, 100.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f,
        -1.0f, 2.0f, 0.0f,
        0.3f, 0.2f, 0.1f
    );
    pointLightCount++;
    pointLights[1] = PointLight(
        1024, 1024, 0.1f, 100.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f,
        -4.0f, 3.0f, 0.0f,
        0.3f, 0.2f, 0.1f
    );
    pointLightCount++;

    spotLightCount = 0;
    spotLights[0] = SpotLight(
        1024, 1024, 0.1f, 100.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        20.0f
    );
    spotLightCount++;

    spotLights[1] = SpotLight(
        1024, 1024, 0.1f, 100.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, -1.5f, 0.0f,
        -100.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        20.0f
    );
    spotLightCount++;

    std::vector<std::string> skyboxFaces;
    // order is important
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

    skybox = Skybox(skyboxFaces);
}

void RenderNormalMapModels(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {

    shaderList[1]->UseShader();

    uniformModel = shaderList[1]->GetModelLocation();
    uniformProjection = shaderList[1]->GetProjectionLocation();
    uniformView = shaderList[1]->GetViewLocation();
    uniformEyePosition = shaderList[1]->GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[1]->GetSpecularIntensityLocation();
    uniformShininess = shaderList[1]->GetShininessLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(
        uniformEyePosition,
        camera.GetCameraPosition().x,
        camera.GetCameraPosition().y,
        camera.GetCameraPosition().z
    );

    shaderList[1]->SetDirectionalLight(&mainLight);
    // first three texture buffers already in use, so start from unit 4
    shaderList[1]->SetPointLights(pointLights, pointLightCount, 4, 1);
    shaderList[1]->SetSpotLights(spotLights, spotLightCount, 4 + pointLightCount, pointLightCount);
    glm::mat4 lightTransform = mainLight.CalculateLightTransform();
    shaderList[1]->SetDirectionalLightTransform(&lightTransform);

    mainLight.GetShadowMap()->Read(GL_TEXTURE3);
    shaderList[1]->SetTexture(1);
    shaderList[1]->SetNormalMap(2);
    shaderList[1]->SetDirectionalShadowMap(3);

    glm::vec3 lowerLight = camera.GetCameraPosition();
    lowerLight.y -= 1.3f;
    lowerLight.x += 1.3f;
    spotLights[1].SetFlash(lowerLight, camera.GetCameraDirection());

    shaderList[1]->Validate();

    glm::mat4 model(1.0f);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(catAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, -1.0f));
    //model = glm::rotate(model, -glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

    brickModel.RenderModel();
}

void RenderScene(Shader* shader) {
    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

    meshList[0]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

    meshList[1]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

    meshList[2]->RenderMesh();

    catAngle += 0.1f;
    if (catAngle > 360) {
        catAngle = 0.1f;
    }


    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    //model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(catAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::translate(model, glm::vec3(-3.0f, -1.0f, -1.0f));
    //model = glm::rotate(model, -glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    //glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

    //orangeCat.RenderModel();
    orangeCatRenderObj.Render(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, -glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    skull.RenderModel();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    donut.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light) {
    directionalShadowShader.UseShader();

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.GetModelLocation();
    glm::mat4 lightTransform = light->CalculateLightTransform();
    directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

    directionalShadowShader.Validate();
    RenderScene(&directionalShadowShader);
    
    // Re-bind default frame buffer once we're done
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light) {
    omniShadowShader.UseShader();

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

    glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

    omniShadowShader.Validate();
    RenderScene(&omniShadowShader);

    // Re-bind default frame buffer once we're done
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    glViewport(0, 0, 1920, 1080);

    // clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    skybox.DrawSkybox(viewMatrix, projectionMatrix);

    shaderList[0]->UseShader();

    uniformModel = shaderList[0]->GetModelLocation();
    uniformProjection = shaderList[0]->GetProjectionLocation();
    uniformView = shaderList[0]->GetViewLocation();
    uniformEyePosition = shaderList[0]->GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
    uniformShininess = shaderList[0]->GetShininessLocation();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(
        uniformEyePosition,
        camera.GetCameraPosition().x,
        camera.GetCameraPosition().y,
        camera.GetCameraPosition().z
    );

    shaderList[0]->SetDirectionalLight(&mainLight);
    // first two texture buffers already in use, so start from unit 3
    shaderList[0]->SetPointLights(pointLights, pointLightCount, 3, 0);
    shaderList[0]->SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

    glm::mat4 lightTransform = mainLight.CalculateLightTransform();
    shaderList[0]->SetDirectionalLightTransform(&lightTransform);

    // Bind to TEXTURE1 since TEXTURE0 is being used by the texture in the main frag shader
    mainLight.GetShadowMap()->Read(GL_TEXTURE2);
    shaderList[0]->SetTexture(1);
    shaderList[0]->SetDirectionalShadowMap(2);

    glm::vec3 lowerLight = camera.GetCameraPosition();
    lowerLight.y -= 0.3f;
    lowerLight.x += 0.3f;
    spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection());

    shaderList[0]->Validate();

    RenderScene(shaderList[0]);
}

void PerformRenderPasses(glm::mat4 projection) {
    DirectionalShadowMapPass(&mainLight);
    for (size_t i = 0; i < pointLightCount; i++) {
        OmniShadowMapPass(&pointLights[i]);
    }
    for (size_t i = 0; i < spotLightCount; i++) {
        OmniShadowMapPass(&spotLights[i]);
    }
    RenderPass(projection, camera.CalculateViewMatrix());
    RenderNormalMapModels(projection, camera.CalculateViewMatrix());
}

int main() {
    
    SetupObjects();
    printf("objects set up\n");
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        (GLfloat)mainWindow.getBufferWidth()/(GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

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

        PerformRenderPasses(projection);

        mainWindow.SwapBufffers();
    }

    return 0;
}