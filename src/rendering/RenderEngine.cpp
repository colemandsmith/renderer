#include "RenderEngine.h"

RenderEngine::RenderEngine(GraphicsApi graphicsApi) {
  mainWindow = std::unique_ptr<Window>(new Window(1366, 768));
  mainWindow->Initialize(graphicsApi);

  currentApi = graphicsApi;
  InitializeRenderer(graphicsApi);
  if (renderer.get() == nullptr) {
    throw std::runtime_error("Renderer could not be initialized");
  }
  shaderManager = std::unique_ptr<ShaderManager>();
  // TODO figure out how to set up window w/ some API abstraction
  // renderer->SetupViewport(mainWindow.get());
  renderer->ClearWindow();

  glm::mat4 projection =
      glm::perspective(glm::radians(60.0f),
                       (float)mainWindow->GetBufferWidth() /
                           (float)mainWindow->GetBufferHeight(),
                       0.1f, 100.0f);
}

void RenderEngine::InitializeRenderer(GraphicsApi graphicsApi) {
  switch (graphicsApi) {
    case GraphicsApi::OPENGL:
      renderer = std::unique_ptr<OpenGLRenderer>(new OpenGLRenderer());
      break;
    case GraphicsApi::NONE:
      std::cout << "Provided NONE API, renderer will not be initialized" << std::endl;
      break;
  }
}

void RenderEngine::Render() {
  // TODO shadow map passes
  MainRenderPass();
  renderer->ClearWindow();
}

void RenderEngine::MainRenderPass() {

}

RenderEngine::~RenderEngine() {
  renderer.reset();
  shaderManager.reset();
  mainWindow.reset();
}
