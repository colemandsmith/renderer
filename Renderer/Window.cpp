#include "Window.h"

Window::Window() {
	width = 1600;
	height = 1200;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

    xChange = 0.0f;
    yChange = 0.0f;
}

int Window::Initialize() {
    // Initialize GLFW
    if (!SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL initialization failed");
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3 );

    mainWindow = SDL_CreateWindow(
        "Main Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!mainWindow) {
        printf("SDL window creation failed!");
        SDL_Quit();
        return -1;
    }

    SDL_GL_GetDrawableSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    context = SDL_GL_CreateContext(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("GLEW initalization failed!");
        SDL_DestroyWindow(mainWindow);
        SDL_Quit();
        return -1;
    }

    // sync with monitor vertical refresh
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Set up viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    SDL_SetWindowGrab(mainWindow, SDL_TRUE);

}

void Window::handleKeys(SDL_Event event) {
    switch(event.type) {
        case SDL_KEYDOWN:
            keys[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            keys[event.key.keysym.sym] = false;
            break;
    }
}

void Window::handleMouse(SDL_Event event) {

   /* if (mouseFirstMoved) {
        lastX = xPos;
        lastY = yPos;
        mouseFirstMoved = false;
    }

    xChange = xPos - lastX;
    yChange = lastY - yPos;

    lastX = xPos;
    lastY = yPos;*/
}

GLfloat Window::getXChange() {
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange() {
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

Window::~Window() {

}