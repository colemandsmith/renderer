#include "RenderEngine.h"

RenderEngine::RenderEngine(GraphicsApi graphicsApi) {
  mainWindow = std::unique_ptr<Window>(new Window(1366, 768));
  mainWindow->Initialize();

  currentApi = graphicsApi;
  InitializeRenderer(graphicsApi);
  if (renderer.get() == nullptr) {
    throw std::runtime_error("Renderer could not be initialized");
  }
  shaderManager = std::unique_ptr<ShaderManager>();
  renderer->SetupViewport(mainWindow.get());
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

RenderEngine::~RenderEngine() {
  renderer.reset();
  shaderManager.reset();
  mainWindow.reset();
}
