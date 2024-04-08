#pragma once

#include<glad/glad.h>
#include "../stb_image.h"

#include "shader.h"
class Texture
{
private:
	GLuint texture;
	int img_width;
	int img_height;
	int colorChannels;
public:	
	Texture(std::string Tex_Path, bool isPNG);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

public:
	GLuint getTexture() const { return this->texture; }
};

