#include "Model3D.h"

Model3D::Model3D(std::string Mesh_Path, std::string Vert_Path, std::string Frag_Path, std::string Tex_Path,
	glm::vec3 position, float scale)
{
	LoadShaders(Vert_Path, Frag_Path);
	LoadMesh(Mesh_Path);
	LoadVertices();
	LoadTexture(Tex_Path);

	this->position = position;

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;

	this->scale = scale;
	this->IdentityMatrix = glm::mat4(1.0f);
	//SetPosition(glm::vec3(this->posX, this->posY, this->posZ));
}

Model3D::~Model3D()
{

}

void Model3D::LoadShaders(std::string Vert_Path, std::string Frag_Path)
{
	//Load Vertex Shader
	std::ifstream vert_file(Vert_Path);
	std::stringstream vert_buffer;
	vert_buffer << vert_file.rdbuf();
	std::string vert_string = vert_buffer.str();
	const char* vert_source = vert_string.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vert_source, NULL);
	glCompileShader(vertexShader);

	//Check for Vertex Shader Compile Errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
	}

	//Load Fragment Shader
	std::ifstream frag_file(Frag_Path);
	std::stringstream frag_buffer;
	frag_buffer << frag_file.rdbuf();
	std::string frag_string = frag_buffer.str();
	const char* frag_source = frag_string.c_str();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &frag_source, NULL);
	glCompileShader(fragmentShader);

	//Check for Fragment Shader Compile Errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
	}

	//Link Shaders
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);

	//Check for Shader Linking Errors
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader Linking Failed: " << infoLog << std::endl;
	}

	//Delete Shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Model3D::LoadMesh(std::string Mesh_Path)
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

void Model3D::LoadVertices()
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

void Model3D::LoadTexture(std::string Tex_Path)
{
	stbi_set_flip_vertically_on_load(true);

	const char* path = Tex_Path.c_str();
	unsigned char* tex_bytes = stbi_load(path, &this->img_width, &this->img_height, &this->colorChannels, 0);

	glGenTextures(1, &this->texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->img_width, this->img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tex_bytes);
}

void Model3D::Update(GLFWwindow* window, float tDeltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->x += 100.0f * tDeltaTime * -1;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->x += 100.0f * tDeltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->y += 100.0f * tDeltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->y += 100.0f * tDeltaTime * -1;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		this->z += 100.0f * tDeltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		this->z += 100.0f * tDeltaTime * -1;
	}
}

glm::mat4 Model3D::TransformationMatrix() const
{
	glm::mat4 TransformationMatrix = glm::translate(this->IdentityMatrix, glm::vec3(position));
	TransformationMatrix = glm::scale(TransformationMatrix, glm::vec3(scale, scale, scale));
	TransformationMatrix = glm::rotate(TransformationMatrix, glm::radians(x), glm::vec3(1.f, 0.f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, glm::radians(y), glm::vec3(0.f, 1.f, 0.f));
	TransformationMatrix = glm::rotate(TransformationMatrix, glm::radians(z), glm::vec3(0.f, 0.f, 1.f));
	return TransformationMatrix;
}

void Model3D::Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, glm::vec3 ambientColor, float specStr, float specPhong, float brightness, glm::vec3 dir_direction,
	glm::vec3 dir_light_color, float dir_ambient_str, glm::vec3 dir_ambient_color,
	float dir_spec_str, float dir_spec_phong, float dir_intensity, glm::vec3 cameraPos)
{
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

	GLuint tex0Address = glGetUniformLocation(this->shaderProgram, "tex0");
	glBindTexture(GL_TEXTURE_2D, this->texture);

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

	glUniform1i(tex0Address, 0);

	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
}

void Model3D::Draw(glm::mat4 transformationMatrix, glm::mat4 view_matrix, glm::mat4 projection_matrix, glm::vec3 light_color)
{
	//glm::mat4 transformationMatrix = this->TransformationMatrix();

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

	/*GLuint tex0Address = glGetUniformLocation(this->shaderProgram, "tex0");
	glBindTexture(GL_TEXTURE_2D, this->texture);*/

	GLuint lightColorAdd = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3fv(lightColorAdd, 1, glm::value_ptr(light_color));

	/*glUniform1i(tex0Address, 0);*/

	glUseProgram(this->shaderProgram);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
}

void Model3D::SetScale(glm::vec3 scale)
{

}

void Model3D::SetPosition(glm::vec3 position)
{

}
