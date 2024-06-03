#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


typedef int ShaderId;

class Shader {
public:
  Shader();

  void CreateFromString(const char *vertexCode, const char *fragmentCode);
  void CreateFromFiles(const char *vertexLocation,
                       const char *fragmentLocation);
  void CreateFromFiles(const char *vertexLocation, const char *geometryLocation,
                       const char *fragmentLocation);
  void Validate();

  std::string ReadFile(const char *fileLocation);

  GLuint GetProjectionLocation() const;
  GLuint GetModelLocation() const;
  GLuint GetViewLocation() const;
  GLuint GetAmbientIntensityLocation() const;
  GLuint GetAmbientColorLocation() const;
  GLuint GetDiffuseIntensityLocation() const;
  GLuint GetDirectionLocation() const;
  GLuint GetEyePositionLocation() const;
  GLuint GetSpecularIntensityLocation() const;
  GLuint GetShininessLocation() const;
  GLuint GetOmniLightPosLocation() const;
  GLuint GetFarPlaneLocation() const;

  void SetDirectionalLight(DirectionalLight *dLight);
  /**  Set the point lights for the shader.
   * texture unit refers to the initial texture
   * offset refers to the offset in OmniShadowMaps
   */
  void SetPointLights(PointLight *pLight, unsigned int lightCount,
                      int textureUnit, unsigned int offset);
  void SetSpotLights(SpotLight *sLight, unsigned int spotLightCount,
                     int textureUnit, unsigned int offset);
  void SetTexture(GLuint textureUnit);
  void SetNormalMap(GLuint textureUnit);
  void SetDirectionalShadowMap(GLuint dShadowMap);
  void SetDirectionalLightTransform(glm::mat4 *lTransform);
  void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

  const inline GLuint GetShaderId() { return shaderId; }

  void UseShader();
  void ClearShader();

  ~Shader();

private:
  int pointLightCount;
  int spotLightCount;

  GLuint shaderId, uniformProjection, uniformModel, uniformView,
      uniformEyePosition, uniformSpecularIntensity, uniformShininess,
      uniformTexture, uniformNormalMap, uniformDirectionalLightTransform,
      uniformDirectionalShadowMap, uniformOmniLightPos, uniformFarPlane;

  GLuint uniformLightMatrices[6];

  struct {
    GLuint uniformColor;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformDirection;
  } uniformDirectionalLight;

  GLuint uniformPointLightCount;

  struct {
    GLuint uniformColor;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;
  } uniformPointLight[MAX_POINT_LIGHTS];

  GLuint uniformSpotLightCount;

  struct {
    GLuint uniformColor;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;

    GLuint uniformDirection;
    GLuint uniformEdge;
  } uniformSpotLight[MAX_SPOT_LIGHTS];

  struct {
    GLuint shadowMap;
    GLuint farPlane;
  } uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

  void CompileShader(const char *vertexCode, const char *fragmentCode);
  void CompileShader(const char *vertexCode, const char *geometryCode,
                     const char *fragmentCode);
  void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);

  void CompileProgram();
};
