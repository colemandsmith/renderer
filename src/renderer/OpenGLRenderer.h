#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "RenderObject.h"
#include "SpotLight.h"
#include "Camera.h"
#include "CommonValues.h"

class OpenGLRenderer {
public:
    OpenGLRenderer();

    void PerformRenderPasses(
        const glm::mat4& projection,
        const Camera* camera,
        const std::vector<RenderObject*> renderObjects,
        const DirectionalLight* mainLight,
        const PointLight (&pointLights)[MAX_POINT_LIGHTS],
        const int pointLightCount,
        const SpotLight (&spotLights)[MAX_SPOT_LIGHTS],
        const int spotLightCount
    );
    
private:
    void RenderScene(Shader* shader);
    void DirectionalShadowMapPass(DirectionalLight* light);
    void OmniShadowMapPass(PointLight* light);
    void VanillaRenderPass(glm::mat4 projection, glm::mat4 viewMatrix);
    void NormalMapRenderPass(glm::mat4 projection, glm::mat4 viewMatrix);
};
