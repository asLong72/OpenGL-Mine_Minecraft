#include "TextureBuffer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb-master/stb_image.h"

TextureBuffer::TextureBuffer(std::string Texture_path) {
	is_array = false;
	glGenTextures(1, &this->TexeturreID);
	glBindTexture(GL_TEXTURE_2D, this->TexeturreID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height, nrChannels;
	GLenum format = 0;
	unsigned char* data = stbi_load(Texture_path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		switch (nrChannels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

TextureBuffer::TextureBuffer(std::vector<std::string>& Texture_path) {
	is_array = true;
	glGenTextures(1, &this->TexeturreID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->TexeturreID);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width = 0, height = 0, nrChannels = 0, texture_NUM = Texture_path.size(), i = 0;
	GLenum format = 0;
	unsigned char* data = NULL;
	while (i < texture_NUM)
	{
		data = stbi_load(Texture_path[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			switch (nrChannels)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				format = 0;
				break;
			}

			if (i == 0)
			{
				glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, texture_NUM, 0, format, GL_UNSIGNED_BYTE, NULL);
			}
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, data);
			std::cerr << Texture_path[i].c_str() << std::endl;;
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		}
		else
		{
			std::cerr << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		i++;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, NULL);
}