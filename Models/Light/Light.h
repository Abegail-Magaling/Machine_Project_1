#pragma once


#include "../GameObject.h"

class Light : public GameObject
{
	private:
		glm::vec3 lightColor;
		glm::vec3 ambientColor;
		float ambientStr;
		float specStr;
		float specPhong;
		float intensity;
		
	protected:
		glm::vec3 position;
	public:
		Light(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, glm::vec3 lightColor, glm::vec3 ambientColor, float ambientStr,
		float specStr, float specPhong);

		virtual glm::vec3 GetLightColor(bool isCenter);
		virtual glm::vec3 GetAmbientColor(bool isCenter);

		inline float GetAmbientStr() { return this->ambientStr; }
		inline float GetSpecStr() { return this->specStr; }
		inline float GetSpecPhong() { return this->specPhong; }
		inline float GetIntensity() { return this->intensity; }

		void LightMovement(glm::vec3 position) { this->position += position; }
		void SetLight(glm::vec3 lightColor) { this->lightColor = lightColor; }
		void SetAmbientColor(glm::vec3 ambientColor) { this->ambientColor = ambientColor; }
		void SetAmbientStr(float ambientStr) { this->ambientStr = ambientStr; }
		void SetSpecStr(float specStr) { this->specStr = specStr; }
		void SetSpecPhong(float specPhong) { this->specPhong = specPhong; }
		void IncreaseIntensity(float intensity) { this->intensity += intensity; }
	};



