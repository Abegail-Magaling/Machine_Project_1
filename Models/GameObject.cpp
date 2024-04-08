#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "GameObject.h"

GameObject::GameObject(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, 
	glm::vec3 position, float scale, bool isPlayer, bool isPNG)
{
	this->isPlayer = isPlayer;
	this->isPNG = isPNG;

	Shader* shader = new Shader(Vert_Path, Frag_Path);
	Texture* bodyTexture = new Texture(Tex_Path, isPNG);
	if (this->isPlayer) {
		Texture* roughnessTexture = new Texture("3D/textures/roughness.jpg", false);
		Texture* metallicsTexture = new Texture("3D/textures/metallics.jpg", false);
		Texture* AOTexture = new Texture("3D/textures/AO.jpg", false);
		Texture* normalTexture = new Texture("3D/Textures/normal.jpg", false);
		this->roughnessTexture = roughnessTexture->getTexture();
		this->metallicsTexture = metallicsTexture->getTexture();
		this->AOTex = AOTexture->getTexture();
		this->normalTexture = normalTexture->getTexture();
	}
	else
	{
		this->roughnessTexture = 0;
		this->metallicsTexture = 0;
		this->AOTex = 0;
		this->normalTexture = 0;
	}
	LoadMesh(Mesh_Path);
	LoadVertices();
	this->shaderProgram = shader->getShaderProg();

	this->bodyTexture = bodyTexture->getTexture();
	
	this->position = position;

	this->x = 0.0f;
	this->y = 180.0f;
	this->z = 0.0f;

	this->scale = scale;
	this->IdentityMatrix = glm::mat4(1.0f);

	//SetPosition(glm::vec3(this->posX, this->posY, this->posZ));
}

GameObject::GameObject(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path, glm::vec3 position, float scale, bool isPNG)
{
	Shader* shader = new Shader(Vert_Path, Frag_Path);
	Texture* bodyTexture = new Texture(Tex_Path, isPNG);
	LoadMesh(Mesh_Path);
	LoadVertices();
	this->shaderProgram = shader->getShaderProg();

	this->bodyTexture = bodyTexture->getTexture();

	this->position = position;

	this->x = 0.0f;
	this->y = 180.0f;
	this->z = 0.0f;

	this->scale = scale;
	this->IdentityMatrix = glm::mat4(1.0f);

	//SetPosition(glm::vec3(this->posX, this->posY, this->posZ));	
}


GameObject::~GameObject()
{

}

void GameObject::LoadMesh(std::string Mesh_Path)
{
	std::string path = Mesh_Path;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	bool success = tinyobj::LoadObj(
		&this->attributes,
		&shapes,
		&material,
		&warning,
		&error,
		path.c_str());

	if (!success)
	{
		std::cout << "FAILED TO LOAD MESH: " << Mesh_Path << std::endl;
	}

	for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
	{
		this->mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
	{
		tinyobj::index_t vData = shapes[0].mesh.indices[i];

		fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);      // X pos
		fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);  // Y pos
		fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);  // Z pos

		fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);       // R nor
		fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);   // S nor
		fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);   // T nor

		fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);       // U tex
		fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);   // V tex
	}
}

void GameObject::LoadVertices()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glUseProgram(this->shaderProgram);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * this->fullVertexData.size(), this->fullVertexData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);

	GLuint normalsPtr = 3 * sizeof(GLfloat);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)normalsPtr);

	GLuint uvPtr = 6 * sizeof(GLfloat);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)uvPtr);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//void GameObject::Update(GLFWwindow* window, float tDeltaTime)
//{
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//	{
//		this->x += 100.0f * tDeltaTime * -1;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//	{
//		this->x += 100.0f * tDeltaTime;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//	{
//		this->y += 100.0f * tDeltaTime;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//	{
//		this->y += 100.0f * tDeltaTime * -1;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//	{
//		this->z += 100.0f * tDeltaTime;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//	{
//		this->z += 100.0f * tDeltaTime * -1;
//	}
//}

glm::mat4 GameObject::TransformationMatrix() const
{
	glm::mat4 TransformationMatrix = glm::translate(this->IdentityMatrix, glm::vec3(position));
	TransformationMatrix = glm::scale(TransformationMatrix, glm::vec3(scale, scale, scale));
	TransformationMatrix = glm::rotate(TransformationMatrix, glm::radians(x), glm::vec3(1.f, 0.f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, glm::radians(y), glm::vec3(0.f, 1.f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, glm::radians(z), glm::vec3(0.f, 0.f, 1.f));
	return TransformationMatrix;
}

void GameObject::DrawPlayer(glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, glm::vec3 ambientColor, float specStr, float specPhong, float brightness, glm::vec3 dir_direction,
	glm::vec3 dir_light_color, float dir_ambient_str, glm::vec3 dir_ambient_color,
	float dir_spec_str, float dir_spec_phong, float dir_intensity, glm::vec3 cameraPos)
{
	glUseProgram(this->shaderProgram);

	glm::mat4 transformationMatrix = this->TransformationMatrix();

	//Get location of projection matrix
	unsigned int projectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
	//Assign the matrix
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	unsigned int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	//Get location of transformation matrix
	unsigned int transformLoc = glGetUniformLocation(this->shaderProgram, "transform");
	//Assign the matrix
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

	glUniform1i(glGetUniformLocation(this->shaderProgram, "tex0"), 0);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "roughnessTex"), 1);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "metallicsTex"), 2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "AOTex"), 3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "normTex"), 4);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->bodyTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->roughnessTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->metallicsTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->AOTex);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->normalTexture);
		
	GLuint lightAddress = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

	GLuint lightColorAdd = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3fv(lightColorAdd, 1, glm::value_ptr(lightColor));

	GLuint ambientStrAdd = glGetUniformLocation(this->shaderProgram, "ambientStr");
	glUniform1f(ambientStrAdd, ambientStr);

	GLuint ambientColorAdd = glGetUniformLocation(this->shaderProgram, "ambientColor");
	glUniform3fv(ambientColorAdd, 1, glm::value_ptr(ambientColor));

	GLuint cameraPosAdd = glGetUniformLocation(this->shaderProgram, "cameraPos");
	glUniform3fv(cameraPosAdd, 1, glm::value_ptr(cameraPos));

	GLuint specStrAdd = glGetUniformLocation(this->shaderProgram, "specStr");
	glUniform1f(specStrAdd, specStr);

	GLuint specPhongAdd = glGetUniformLocation(this->shaderProgram, "specPhong");
	glUniform1f(specPhongAdd, specPhong);

	GLuint brightnessAdd = glGetUniformLocation(this->shaderProgram, "brightness");
	glUniform1f(brightnessAdd, brightness);

	GLuint dirLightAddress = glGetUniformLocation(this->shaderProgram, "direction");
	glUniform3fv(dirLightAddress, 1, glm::value_ptr(dir_direction));

	GLuint dirLightColorAddress = glGetUniformLocation(this->shaderProgram, "dirLightColor");
	glUniform3fv(dirLightColorAddress, 1, glm::value_ptr(dir_light_color));

	GLuint dirAmbientStrAddress = glGetUniformLocation(this->shaderProgram, "dirAmbientStr");
	glUniform1f(dirAmbientStrAddress, dir_ambient_str);

	GLuint dirAmbientColorAddress = glGetUniformLocation(this->shaderProgram, "dirAmbientColor");
	glUniform3fv(dirAmbientColorAddress, 1, glm::value_ptr(dir_ambient_color));

	GLuint dirSpecStrAddress = glGetUniformLocation(this->shaderProgram, "dirSpecStr");
	glUniform1f(dirSpecStrAddress, dir_spec_str);

	GLuint dirSpecPhongAddress = glGetUniformLocation(this->shaderProgram, "dirSpecPhong");
	glUniform1f(dirSpecPhongAddress, dir_spec_phong);

	GLuint intensityAddress = glGetUniformLocation(this->shaderProgram, "dirIntensity");
	glUniform1f(intensityAddress, dir_intensity);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
}

void GameObject::Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, glm::vec3 ambientColor, float specStr, float specPhong, float brightness, glm::vec3 dir_direction,
	glm::vec3 dir_light_color, float dir_ambient_str, glm::vec3 dir_ambient_color,
	float dir_spec_str, float dir_spec_phong, float dir_intensity, glm::vec3 cameraPos)
{
	glUseProgram(this->shaderProgram);

	glm::mat4 transformationMatrix = this->TransformationMatrix();

	//Get location of projection matrix
	unsigned int projectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
	//Assign the matrix
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	unsigned int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

	//Get location of transformation matrix
	unsigned int transformLoc = glGetUniformLocation(this->shaderProgram, "transform");
	//Assign the matrix
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

	glUniform1i(glGetUniformLocation(this->shaderProgram, "tex0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->bodyTexture);

	GLuint lightAddress = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

	GLuint lightColorAdd = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3fv(lightColorAdd, 1, glm::value_ptr(lightColor));

	GLuint ambientStrAdd = glGetUniformLocation(this->shaderProgram, "ambientStr");
	glUniform1f(ambientStrAdd, ambientStr);

	GLuint ambientColorAdd = glGetUniformLocation(this->shaderProgram, "ambientColor");
	glUniform3fv(ambientColorAdd, 1, glm::value_ptr(ambientColor));

	GLuint cameraPosAdd = glGetUniformLocation(this->shaderProgram, "cameraPos");
	glUniform3fv(cameraPosAdd, 1, glm::value_ptr(cameraPos));

	GLuint specStrAdd = glGetUniformLocation(this->shaderProgram, "specStr");
	glUniform1f(specStrAdd, specStr);

	GLuint specPhongAdd = glGetUniformLocation(this->shaderProgram, "specPhong");
	glUniform1f(specPhongAdd, specPhong);

	GLuint brightnessAdd = glGetUniformLocation(this->shaderProgram, "brightness");
	glUniform1f(brightnessAdd, brightness);

	GLuint dirLightAddress = glGetUniformLocation(this->shaderProgram, "direction");
	glUniform3fv(dirLightAddress, 1, glm::value_ptr(dir_direction));

	GLuint dirLightColorAddress = glGetUniformLocation(this->shaderProgram, "dirLightColor");
	glUniform3fv(dirLightColorAddress, 1, glm::value_ptr(dir_light_color));

	GLuint dirAmbientStrAddress = glGetUniformLocation(this->shaderProgram, "dirAmbientStr");
	glUniform1f(dirAmbientStrAddress, dir_ambient_str);

	GLuint dirAmbientColorAddress = glGetUniformLocation(this->shaderProgram, "dirAmbientColor");
	glUniform3fv(dirAmbientColorAddress, 1, glm::value_ptr(dir_ambient_color));

	GLuint dirSpecStrAddress = glGetUniformLocation(this->shaderProgram, "dirSpecStr");
	glUniform1f(dirSpecStrAddress, dir_spec_str);

	GLuint dirSpecPhongAddress = glGetUniformLocation(this->shaderProgram, "dirSpecPhong");
	glUniform1f(dirSpecPhongAddress, dir_spec_phong);

	GLuint intensityAddress = glGetUniformLocation(this->shaderProgram, "dirIntensity");
	glUniform1f(intensityAddress, dir_intensity);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
}

void GameObject::SetScale(glm::vec3 scale)
{

}

void GameObject::SetPosition(glm::vec3 position)
{
	this->position = position;
}
