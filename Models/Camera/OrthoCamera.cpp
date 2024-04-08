#include "OrthoCamera.h"

OrthoCamera::OrthoCamera() : MyCamera()
{
	this->projectionMatrix = projectionMatrix;
	projectionMatrix = glm::ortho(-15.f, 15.f, -15.f, 15.f, -1.f, 100.f);
	setCameraPosition(glm::vec3(0.0f, 30.0f, 5.0f));
	setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
}

glm::mat4 OrthoCamera::getProjectionMatrix(float aspectRatio) const
{
	return projectionMatrix;
}