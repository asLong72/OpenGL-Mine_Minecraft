#pragma once
#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H

#include <iostream>
#include <vector>
#include "../include/glad/glad.h"
#include "../include/GLFW/include/glfw3.h"

class TextureBuffer {
public:
	TextureBuffer() { TexeturreID = 0; is_array = false; };
	TextureBuffer(std::string);
	TextureBuffer(std::vector<std::string>&);
	inline void Enabletexture() { glBindTexture(is_array ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D, TexeturreID); };
	inline void Disabletexture() { glBindTexture(is_array ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D, NULL); };
	~TextureBuffer() { };

private:
	bool is_array;
	unsigned TexeturreID;
};
#endif // !TEXTUREBUFFER_H
