#pragma once

#include "../GameObject.h"

class MyCamera
{
protected:
    glm::vec3 cameraPos;
    glm::vec3 orientation;
    glm::vec3 up;
    glm::mat4 view_matrix;
public:
    MyCamera();

    void CameraMovement(glm::vec3 movement);
    void setCameraPosition(glm::vec3 position);
    void setOrientation(glm::vec3 orientation);

private:
    void SetViewMatrix();
public:
    glm::mat4 getViewMatrix();
    glm::vec3 getCameraPosition();
    virtual glm::mat4 getProjectionMatrix(float aspectRatio) const = 0;
    //virtual void Update(float deltaTime) = 0;
};