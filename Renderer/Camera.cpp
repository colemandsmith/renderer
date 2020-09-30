#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition,
			   glm::vec3 startUp, 
			   GLfloat startYaw,
			   GLfloat startPitch,
			   GLfloat startMoveSpeed,
			   GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
}

void Camera::KeyControl(std::map<char, bool> keys, GLfloat deltaTime) {

    printf("delta time %f\n", deltaTime);

    printf("move speed %f\n", moveSpeed);
    GLfloat velocity = moveSpeed * deltaTime;
    printf("velocity %f\n", velocity);
    if (keys[SDLK_w]) {
        printf("front %f %f %f\n", front.x, front.y, front.z);
        position += front * velocity;
    }
    if (keys[SDLK_s]) {
        position -= front * velocity;
    }
    if (keys[SDLK_a]) {
        position -= right * velocity;
    }
    if (keys[SDLK_d]) {
        position += right * velocity;
    }
    if (keys[SDLK_SPACE]) {
        position += up * velocity;
    }
    if (keys[SDLK_LCTRL]) {
        position -= up * velocity;
    }
    printf("position %f %f %f\n", position.x, position.y, position.z);
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();
}

glm::vec3 Camera::GetCameraPosition() {
	return position;
}

glm::vec3 Camera::GetCameraDirection() {
    return glm::normalize(front);
}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera() {

}