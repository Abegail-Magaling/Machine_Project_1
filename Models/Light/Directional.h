#pragma once
#include "Light.h"

class Directional : public Light
{
private:
    glm::vec3 direction;
    glm::vec3 lightColor;
    glm::vec3 ambientColor;
    const float intensitySpeed = 2.0f;

public:
    Directional(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr,
        float specStr, float specPhong);

    glm::vec3 GetDirection() { return this->direction; }
    void SetDirection(glm::vec3 direction) { this->direction = direction; }

    void Update(GLFWwindow*, float tDeltaTime);

    glm::vec3 GetLightColor(bool isPerspective) override { return this->lightColor; }
    glm::vec3 GetAmbientColor(bool isPerspective) override { return this->ambientColor; }

};





