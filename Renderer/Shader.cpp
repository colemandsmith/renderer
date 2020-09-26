#include "Shader.h"


Shader::Shader() {
	shaderId = 0;
	uniformModel = 0;
	uniformProjection = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation);
    std::string geometryString = ReadFile(geometryLocation);
    std::string fragmentString = ReadFile(fragmentLocation);

    const char* vertexCode = vertexString.c_str();
    const char* geometryCode = geometryString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::Validate() {
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

std::string Shader::ReadFile(const char* fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesn't exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

void Shader::CompileProgram() {
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformView = glGetUniformLocation(shaderId, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightCount");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderId, locBuff);

    }

    uniformTexture = glGetUniformLocation(shaderId, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderId, "directionalLightSpaceTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderId, "directionalShadowMap");

    uniformOmniLightPos = glGetUniformLocation(shaderId, "lightPos");
    uniformFarPlane = glGetUniformLocation(shaderId, "farPlane");

    for (size_t i = 0; i < 6; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(shaderId, locBuff);
    }

    for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderId, locBuff);
    }
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("Error creating shader program!");
        return;
    }

    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode) {
    shaderId = glCreateProgram();

    if (!shaderId) {
        printf("Error creating shader program!");
        return;
    }

    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderId, geometryCode, GL_GEOMETRY_SHADER);
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, 1024, NULL, eLog);
        fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

GLuint Shader::GetProjectionLocation() {
    return uniformProjection;
}

GLuint Shader::GetModelLocation() {
    return uniformModel;
}

GLuint Shader::GetEyePositionLocation() {
    return uniformEyePosition;
}

GLuint Shader::GetViewLocation() {
    return uniformView;
}

GLuint Shader::GetAmbientColorLocation() {
    return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetAmbientIntensityLocation() {
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation() {
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation() {
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation() {
    return uniformShininess;
}

GLuint Shader::GetOmniLightPosLocation() {
    return uniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation() {
    return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight) {
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
                     uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount, int textureUnit, unsigned int offset) {
    if (lightCount > MAX_POINT_LIGHTS) {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity,
                           uniformPointLight[i].uniformColor,
                           uniformPointLight[i].uniformDiffuseIntensity,
                           uniformPointLight[i].uniformPosition,
                           uniformPointLight[i].uniformConstant,
                           uniformPointLight[i].uniformLinear,
                           uniformPointLight[i].uniformExponent);

        // dynamically set which texture we want to get
        pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
    }
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int spotLightCount, int textureUnit, unsigned int offset) {
    if (spotLightCount > MAX_POINT_LIGHTS) {
        spotLightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformSpotLightCount, spotLightCount);

    for (size_t i = 0; i < spotLightCount; i++) {
        sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity,
                           uniformSpotLight[i].uniformColor,
                           uniformSpotLight[i].uniformDiffuseIntensity,
                           uniformSpotLight[i].uniformPosition,
                           uniformSpotLight[i].uniformDirection,
                           uniformSpotLight[i].uniformConstant,
                           uniformSpotLight[i].uniformLinear,
                           uniformSpotLight[i].uniformExponent,
                           uniformSpotLight[i].uniformEdge
        );

        // dynamically set which texture we want to get
        sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane());
    }
}

void Shader::SetTexture(GLuint textureUnit) {
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint dShadowMap) {
    glUniform1i(uniformDirectionalShadowMap, dShadowMap);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform) {
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices) {
    for (size_t i = 0; i < 6; i++) {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
}

void Shader::UseShader() {
    glUseProgram(shaderId);
}

void Shader::ClearShader() {
    if (shaderId != 0) {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader() {
    ClearShader();
}