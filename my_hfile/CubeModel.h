#pragma once
#ifndef CUBEMODEL_H
#define CUBEMODEL_H

#include <iostream>
#include <vector>
#include "Shader.h"

class CubeModel {
public:
	CubeModel();
	CubeModel(std::string, std::string);
	inline void UsingModel() {
		glBindVertexArray(VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	};
	inline void DisableModel() {
		glBindVertexArray(NULL);
		//glBindBuffer(GL_ARRAY_BUFFER, NULL);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	};
	inline unsigned getVAO() { return VAO; };
	inline unsigned getVBO() { return VBO; };
	inline unsigned getEBO() { return EBO; };
	~CubeModel() { };


	static Shader cube_shader;
protected:

	unsigned VAO;
	unsigned VBO;
	unsigned EBO;
};
#endif // !CUBEMODEL_H