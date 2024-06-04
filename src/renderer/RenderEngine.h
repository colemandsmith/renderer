#pragma once

#include <memory>

#include "GraphicsApi.h"
#include "OpenGLRenderer.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Window.h"

class RenderEngine {
public:
  RenderEngine(GraphicsApi graphicsApi = GraphicsApi::OPENGL);
  ~RenderEngine();

  Renderer *GetRenderer() { return renderer.get(); }
  Window *GetWindow() { return mainWindow.get(); }

  // Perform forward rendering
  void Render();

private:
  void InitializeRenderer(GraphicsApi graphicsApi);
  // Perform the render pass that uses materials
  void MainRenderPass();

  glm::mat4 projection;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<ShaderManager> shaderManager;
  std::unique_ptr<Window> mainWindow;
  GraphicsApi currentApi;
};
