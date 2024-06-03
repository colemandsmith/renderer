#include "Window.h"

Window::Window() {
	width = 1600;
	height = 1200;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    xChange = 0.0f;
    yChange = 0.0f;
}

Window::Window(int windowWidth, int windowHeight) {
	width = windowWidth;
	height = windowHeight;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    xChange = 0.0f;
    yChange = 0.0f;
}

int Window::Initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW initialization failed");
        glfwTerminate();
        return 1;
    }

    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    // Handle key + mouse input
    createCallBacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(mainWindow, this);
    return 1;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

void Window::createCallBacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

float Window::getXChange() {
    float theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

float Window::getYChange() {
    float theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

Window::~Window() {

}
