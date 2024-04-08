#include "Directional.h"

Directional::Directional(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr, float specStr, float specPhong) : 
	Light(Mesh_Path, Vert_Path, Frag_Path, Tex_Path, position, scale, lightColor, ambientColor, ambientStr, specStr, specPhong)
{
	this->direction = glm::normalize(glm::vec3(0.f, 1.f, 0.f));
	this->lightColor = lightColor;
	this->ambientColor = ambientColor;
}

void Directional::Update(GLFWwindow* window, float tDeltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        this->IncreaseIntensity(this->intensitySpeed * tDeltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        this->IncreaseIntensity(this->intensitySpeed * -tDeltaTime);
    }
}
