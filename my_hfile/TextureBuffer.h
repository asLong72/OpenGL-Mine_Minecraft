#pragma once
#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H

#include <iostream>
#include <vector>
#include "../include/glad/glad.h"
#include "../include/GLFW/include/glfw3.h"

class TextureBuffer {
public:
	TextureBuffer() { TexeturreID = 0; };
	TextureBuffer(std::string);
	TextureBuffer(std::vector<std::string>);
	//TextureBuffer(std::vector<std::string>);
	inline void Enabletexture() { glBindTexture(GL_TEXTURE_2D, TexeturreID); };
	inline void Disabletexture() { glBindTexture(GL_TEXTURE_2D, NULL); };
	~TextureBuffer() { };

private:
	unsigned TexeturreID;
};
#endif // !TEXTUREBUFFER_H
