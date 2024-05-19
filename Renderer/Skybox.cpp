#include "Skybox.h"

Skybox::Skybox() {

}

Skybox::Skybox(std::string skyboxLocation, Skybox::TextureType skyboxTextureType) {
    CreateMesh();
    // Shader Location
    skyShader = new Shader();
    skyShader->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

    uniformProjection = skyShader->GetProjectionLocation();
    uniformView = skyShader->GetViewLocation();

    int width, height, nrComponents;

    stbi_set_flip_vertically_on_load(true);
    GLuint equirectTextureId;
    float *data = stbi_loadf(skyboxLocation.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        glGenTextures(1, &equirectTextureId);
        glBindTexture(GL_TEXTURE_2D, equirectTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else { 
      printf("Failed to load %s\n", skyboxLocation.c_str());
    }
    stbi_set_flip_vertically_on_load(false);

    ConvertEquirectToCubemap(equirectTextureId);
}

Skybox::Skybox(std::vector<std::string> faceLocations) {
    CreateMesh();
    skyboxTextureType = Skybox::TextureType::SEPARATE_IMAGES;
    // Shader Location
    skyShader = new Shader();
    skyShader->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

    uniformProjection = skyShader->GetProjectionLocation();
    uniformView = skyShader->GetViewLocation();

    // Texture Setup
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int width, height, bitDepth;

    for (size_t i = 0; i < 6; i++) {
        unsigned char* textureData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
        if (!textureData) {
            printf("Failed to find %s\n", faceLocations[i].c_str());
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        stbi_image_free(textureData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // s
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // t
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // r
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    // want to account for rotation of the camera
    // this cuts off the transform data by converting to 3x3 then back to 4x4 matrix
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));
    
    glDepthMask(GL_FALSE);

    skyShader->UseShader();

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    skyShader->Validate();

    skyMesh->RenderMesh();

    glDepthMask(GL_TRUE);
}

void Skybox::ConvertEquirectToCubemap(GLuint equirectTextureId) {
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    // Set up a cubemap from the equirectangular image
    glGenTextures(1, &equirectTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, equirectTextureId);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set up projection and view matrices to capture the data
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // conversion shaders
    Shader* conversionShader = new Shader();
    conversionShader->CreateFromFiles(
        "Shaders/skybox_equirect.vert",
        "Shaders/skybox_equirect.frag"
    );
    conversionShader->UseShader();

    conversionShader->SetInt("equirectangularMap", 0);
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(captureProjection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, equirectTextureId);

    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, equirectTextureId, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // hack - we already have a cube mesh; use it as the rendering base
        skyMesh->RenderMesh();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // return to original dims
    glViewport(0, 0, 1920, 1080);
}

void Skybox::CreateMesh() {
    // Mesh Setup

    unsigned int skyboxIndices[] = {
        // front
        0, 1, 2,
        2, 1, 3,
        //right
        2, 3, 5,
        5, 3, 7,
        // back
        5, 7, 4,
        4, 7, 6,
        //left
        4, 6, 0,
        0, 6, 1,
        //top
        4, 0, 5,
        5, 0, 2,
        // bottom
        1, 6, 3,
        3, 6, 7
    };

    float skyboxVertices[] = {
        -1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,

        -1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
    };

    skyMesh = new Mesh();
    skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 88, 36);
}

Skybox::~Skybox() {
    // delete skyShader;
}
