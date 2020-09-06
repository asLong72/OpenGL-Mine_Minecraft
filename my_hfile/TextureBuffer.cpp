#include "TextureBuffer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb-master/stb_image.h"

TextureBuffer::TextureBuffer(std::string Texture_path) {
	glGenTextures(1, &this->TexeturreID);
	glBindTexture(GL_TEXTURE_2D, this->TexeturreID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(Texture_path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

TextureBuffer::TextureBuffer(std::vector<std::string> Texture_path) {
	glGenTextures(1, &this->TexeturreID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->TexeturreID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	short count = 0, texture_num = 0;
	int width, height, nrComponents;
	GLenum format = 0;
	unsigned char* data;

	while (count < 6)
	{
		data = stbi_load(Texture_path[texture_num].c_str(), &width, &height, &nrComponents, 0);
		if (data) {
			switch (nrComponents)
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "纹理加载失败，路径是:" << Texture_path[texture_num] << std::endl;
		}
		stbi_image_free(data);
		switch (Texture_path.size())
		{
		case 1:
			texture_num = 0;
			break;
		case 2:
			if (texture_num == 0)
			{
				texture_num = 1;
			}
			break;
		case 3:
			break;
		case 6:
			break;
		default:
			break;
		}
		count++;
	}


	glBindTexture(GL_TEXTURE_2D, NULL);
}