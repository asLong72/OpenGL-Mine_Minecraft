#pragma once
#ifndef __Game_Object__
#define __Game_Object__

//#define STB_IMAGE_IMPLEMENTATION
//预处理器会修改头文件，让其只包含相关的函数定义源码
//等于是将这个头文件变为一个 .cpp 文件了
//现在只需要在你的程序中包含stb_image.h并编译就可以了
#include "../include/stb-master/stb_image.h"

#include "../include/glad/glad.h"
#include "../include/GLFW/include/glfw3.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "iostream"
#include "fstream"
#include "strstream"
#include "vector"

enum class BlockModelsID
{
	Null = 0,
	FullBlock = 1,
	PlantBlock = 2,
	HalfBlock = 3

};

enum class BlockTextureID
{
	Null = 0,
	grass = 1,
	stone = 2,
	dirt = 3,
	wood = 4

};

class Game_Object
{
public:
	virtual int get_ID() = 0;

protected:

};

class Block :public Game_Object
{
public:
	Block(int);
	~Block();
	int get_ID();
	float* get_model();
	int get_arraysize();

protected:
	int block_textureID;
	BlockModelsID block_modelID;
};

class Texture :public Game_Object
{
public:

	Texture();
	~Texture() {};

private:
	unsigned textureID;
};

#endif // !__Game_Object__
