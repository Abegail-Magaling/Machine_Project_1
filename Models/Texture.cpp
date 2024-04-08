#include "Texture.h"

Texture::Texture(std::string Tex_Path, bool isPNG)
{
	stbi_set_flip_vertically_on_load(true);

	const char* path = Tex_Path.c_str();
	unsigned char* tex_bytes = stbi_load(path, &this->img_width, &this->img_height, &this->colorChannels, 0);

	glGenTextures(1, &this->texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	if (isPNG) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->img_width, this->img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->img_width, this->img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tex_bytes);
}
