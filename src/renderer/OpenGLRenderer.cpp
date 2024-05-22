#include "OpenGLRenderer.h"

void OpenGLRenderer::PerformRenderPasses(
        const glm::mat4& projection,
        const Camera* camera,
        const std::vector<RenderObject*> renderObjects,
        const DirectionalLight* mainLight,
        const PointLight (&pointLights)[MAX_POINT_LIGHTS],
        const int pointLightCount,
        const SpotLight (&spotLights)[MAX_SPOT_LIGHTS],
        const int spotLightCount
    ) {
    }
