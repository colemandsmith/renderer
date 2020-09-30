#pragma once

#include <map>
#include <stdio.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Window {
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat getBufferWidth() { return bufferWidth; };
	GLfloat getBufferHeight() { return bufferHeight; };

    std::map<char, bool> getKeys() { return keys;  }
	GLfloat getXChange();
	GLfloat getYChange();

    void handleKeys(SDL_Event event);
    void handleMouse(SDL_Event event);

	void SwapBufffers() { SDL_GL_SwapWindow(mainWindow); }

	~Window();

private:
    SDL_Window* mainWindow;
    SDL_GLContext context;

	GLint width, height;
	GLint bufferHeight, bufferWidth;

	std::map<char, bool> keys;

	Sint32 lastX;
    Sint32 lastY;
    Sint32 xChange;
    Sint32 yChange;
	bool mouseFirstMoved;
};

