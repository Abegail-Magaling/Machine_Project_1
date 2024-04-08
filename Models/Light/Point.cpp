#include "Point.h"

Point::Point(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr, float specStr, float specPhong)
	: Light(Mesh_Path, Vert_Path, Frag_Path, Tex_Path, position, scale, lightColor, ambientColor, ambientStr, specStr, specPhong)
{
}

void Point::Update(GLFWwindow* window, float tDeltaTime)
{

}

void Point::cycleLightIntensity()
{
    // Cycle through intensity levels
    if (this->GetIntensity() < 1.01f) {
        // Low intensity
        this->IncreaseIntensity(3.0f);
    }
    else if (this->GetIntensity() < 3.01f) {
        // Medium intensity
        this->IncreaseIntensity(3.0f);
    }
    else {
        // High intensity
        this->IncreaseIntensity(-6.0f);
    }
}


