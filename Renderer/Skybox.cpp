#include "Skybox.h"

Skybox::Skybox() {

}

Skybox::Skybox(std::vector<std::string> faceLocations) {
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

Skybox::~Skybox() {
    printf("destructing skybox\n");
    // delete skyShader;
}
