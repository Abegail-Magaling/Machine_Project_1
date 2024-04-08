#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../tiny_obj_loader.h"
#include "../stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>


class Model3D
{
private:
	GLuint VAO;
	GLuint VBO;
	GLuint shaderProgram;
	GLuint texture;

	glm::vec3 position;



	float scale;
	tinyobj::attrib_t attributes;
	std::vector<GLuint> mesh_indices;
	std::vector<GLfloat> fullVertexData;

	int img_width;
	int img_height;
	int colorChannels;

	glm::mat4 IdentityMatrix;
protected:

	float x;
	float y;
	float z;

public:

	Model3D(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path,
		glm::vec3 position, float scale);
	~Model3D();

	void Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, glm::vec3 ambientColor, float specStr, float specPhong, float brightness, glm::vec3 dir_direction,
		glm::vec3 dir_light_color, float dir_ambient_str, glm::vec3 dir_ambient_color,
		float dir_spec_str, float dir_spec_phong, float dir_intensity, glm::vec3 cameraPos);
	void Draw(glm::mat4 transformationMatrix, glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 light_color);



	void LoadShaders(std::string Vert_Path, std::string Frag_Path);
	void LoadMesh(std::string Mesh_Path);
	void LoadTexture(std::string Tex_Path);
	void LoadVertices();

	void Update(GLFWwindow* window, float tDeltaTime);

	glm::mat4 TransformationMatrix() const;
	void SetScale(glm::vec3 scale);
	void SetPosition(glm::vec3 position);
	//void Rotate(float angle, glm::vec3 axis);

	inline glm::vec3 GetPosition() { return position; }
	inline GLuint GetVAO() { return VAO; }
	inline GLuint GetVBO() { return VBO; }
	inline GLuint GetShaderProgram() { return shaderProgram; }
	inline GLuint GetTexture() { return texture; }
	inline std::vector<GLuint> GetMeshIndices() { return mesh_indices; }
	inline std::vector<GLfloat> GetFullVertexData() { return fullVertexData; }
	inline int GetImgWidth() { return img_width; }
	inline int GetImgHeight() { return img_height; }

};

