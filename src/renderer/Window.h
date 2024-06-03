#pragma once

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

class Window {
public:
  Window();
  Window(int windowWidth, int windowHeight);

  int Initialize();

  int GetBufferWidth() const { return bufferWidth; };
  int GetBufferHeight() const { return bufferHeight; };
  GLFWwindow* GetGlfwWindow() { return mainWindow; }

  bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }

  const bool *getKeys() const { return keys; }
  float getXChange();
  float getYChange();

  void SwapBufffers() const { glfwSwapBuffers(mainWindow); }

  ~Window();

private:
  GLFWwindow* mainWindow = nullptr;

  int width, height;
  int bufferHeight, bufferWidth;

  bool keys[1024];

  float lastX;
  float lastY;
  float xChange;
  float yChange;
  bool mouseFirstMoved;

  void createCallBacks();
  static void handleKeys(GLFWwindow *window, int key, int code, int action,
                         int mode);
  static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};
