#pragma once

#include <memory>

#include "OpenGLRenderer.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Window.h"

// Note: Currently this project ONLY supports OpenGL, but this
// slight modularity is in hopes of future additions for other
// graphics APIs
enum GraphicsApi { OPENGL, NONE };

class RenderEngine {
public:
  RenderEngine(GraphicsApi graphicsApi = GraphicsApi::OPENGL);
  ~RenderEngine();

  Renderer *GetRenderer() { return renderer.get(); }
  Window *GetWindow() { return mainWindow.get(); }

  void Render();

private:
  void InitializeRenderer(GraphicsApi graphicsApi);
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<ShaderManager> shaderManager;
  std::unique_ptr<Window> mainWindow;
  GraphicsApi currentApi;
};
