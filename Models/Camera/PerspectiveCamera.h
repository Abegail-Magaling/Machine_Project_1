#pragma once

#include "MyCamera.h"
#include "../GameObjects/Player.h"

class PerspectiveCamera : public MyCamera
{
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float offset;
	float window_width;
	float window_height;
	float aspectRatio;
	bool firstClick;
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	float theta;
	float phi;
	float camX;
	float camY;
	float camZ;
	float zFar;
	const float sensitivity = 0.1f;
public:
	PerspectiveCamera();
	glm::mat4 getProjectionMatrix(float aspectRatio) const;
	void SetProjectionMatrix(float viewAngle);
	void updateCamera(GLFWwindow* window, float deltaTime);
	void Update(GLFWwindow* window, float deltaTime, Player* pPlayer);
	void FOV(bool is1stPerson, bool is3rdPerson, glm::vec3 playerposition);
	void setZfar(float zFar);
};

