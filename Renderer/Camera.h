#pragma once

#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/sdl.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	Camera();
	Camera(glm::vec3 startPosition,
	       glm::vec3 startUp,
		   GLfloat startYaw,
		   GLfloat startPitch,
		   GLfloat startMoveSpeed,
		   GLfloat startTurnSpeed);

	void KeyControl(std::map<char, bool> keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 GetCameraPosition();
    glm::vec3 GetCameraDirection();
	glm::mat4 CalculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
    Sint32 xPos;
    Sint32 yPos;
    Sint32 xChange;
    Sint32 yChange;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

