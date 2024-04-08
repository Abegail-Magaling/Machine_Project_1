#pragma once
#include "Light.h"

class Point : public Light
{
	private:
		const float orbitSpeed = 1.0f;
		const float intensitySpeed = 1000.0f;	
	public:
		Point(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr,
			float specStr, float specPhong);

		void Update(GLFWwindow* window, float tDeltaTime);
		void cycleLightIntensity();
};

