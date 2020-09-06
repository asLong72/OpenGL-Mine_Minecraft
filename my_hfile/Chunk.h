#pragma once
#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "CubeModel.h"

typedef struct Block {
	unsigned char up;
	unsigned char down;
	unsigned char right;
	unsigned char left;
	unsigned char front;
	unsigned char behind;
};

class Blocks :public CubeModel
{
public:
	Blocks();
	Blocks(std::string, std::string);
	void Init(unsigned char*);
	void inNeedData_Update();
	void setIntancedVBO();
	GLsizei getNum() { return intance_num; };
	~Blocks() { delete inNeedData; };

protected:
	unsigned char* AllData;
	unsigned char* inNeedData;
	unsigned intancedVBO;
	GLsizei intance_num;
	bool visualable;
};

class Chunks
{
public:
	Chunks() { };
	~Chunks() { };

protected:
	//Blocks blockArray[16];

};

#endif // !__CHUNK_H__
