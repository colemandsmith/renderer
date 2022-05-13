#pragma once

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window {
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

    GLint getBufferWidth() { return bufferWidth; };
    GLint getBufferHeight() { return bufferHeight; };
    static int getFrameCount() { return frameCount; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys;  }
	GLfloat getXChange();
	GLfloat getYChange();

    void SwapBufffers() { Window::frameCount++;  glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferHeight, bufferWidth;
    static int frameCount;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

