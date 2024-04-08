#include "Light.h"


Light::Light(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale,
	glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr, float specStr, float specPhong)
	: GameObject (Mesh_Path, Vert_Path, Frag_Path, Tex_Path, position, scale, isPNG)
{
	//this->SetPosition(pos);
	this->lightColor = lightColor;
	this->ambientColor = ambientColor;
	this->ambientStr = ambientStr;
	this->specStr = specStr;
	this->specPhong = specPhong;
	this->intensity = 1.0f;
	this->position = position;
}

glm::vec3 Light::GetLightColor(bool isCenter)
{
	if (isCenter)
	{
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	else
	{
		lightColor = glm::vec3(153.f / 254.f, 255.f / 254.f, 153.f / 254.f);
	}

	return lightColor;
}

glm::vec3 Light::GetAmbientColor(bool isCenter)
{
	if (isCenter)
	{
		ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	else
	{
		ambientColor = glm::vec3(153.f / 254.f, 255.f / 254.f, 153.f / 254.f);
	}

	return ambientColor;
}