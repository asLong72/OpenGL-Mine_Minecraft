#include "CubeModel.h"

CubeModel::CubeModel() {
	progID = 0;
	VAO = 0;
	VBO = 0;
	EBO = 0;
};
CubeModel::CubeModel(std::string vertexShader_path, std::string fragmentShader_path) :Shader(vertexShader_path, fragmentShader_path) {
	float vertex[]
	{
		//正面
		0.0f,	0.0f,	1.0f,	0.0f,	0.0f,		//左下
		1.0f,	0.0f,	1.0f,	0.0f,	1.0f,		//右下
		1.0f,	1.0f,	1.0f,	1.0f,	1.0f,		//右上
		0.0f,	1.0f,	1.0f,	1.0f,	0.0f,		//左上
		//
		1.0f,	0.0f,	0.0f,	0.0f,	0.0f,		//左下
		0.0f,	0.0f,	0.0f,	0.0f,	1.0f,		//右下
		0.0f,	1.0f,	0.0f,	1.0f,	1.0f,		//右上
		1.0f,	1.0f,	0.0f,	1.0f,	0.0f,		//左上
		//顶面
		0.0f,	1.0f,	1.0f,	0.0f,	0.0f,		//左下
		1.0f,	1.0f,	1.0f,	0.0f,	1.0f,		//右下
		1.0f,	1.0f,	0.0f,	1.0f,	1.0f,		//右上
		0.0f,	1.0f,	0.0f,	1.0f,	0.0f,		//左上
		//
		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,		//左下
		1.0f,	0.0f,	0.0f,	0.0f,	1.0f,		//右下
		1.0f,	0.0f,	1.0f,	1.0f,	1.0f,		//右上
		0.0f,	0.0f,	1.0f,	1.0f,	0.0f,		//左上
		//左面
		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,		//左下
		0.0f,	0.0f,	1.0f,	0.0f,	1.0f,		//右下
		0.0f,	1.0f,	1.0f,	1.0f,	1.0f,		//右上
		0.0f,	1.0f,	0.0f,	1.0f,	0.0f,		//左上
		//
		1.0f,	0.0f,	1.0f,	0.0f,	0.0f,		//左下
		1.0f,	0.0f,	0.0f,	0.0f,	1.0f,		//右下
		1.0f,	1.0f,	0.0f,	1.0f,	1.0f,		//右上
		1.0f,	1.0f,	1.0f,	1.0f,	0.0f,		//左上
	};
	unsigned indicat[] =
	{
		//
		0,	1,	3,
		1,	2,	3,
		//
		4,	5,	7,
		5,	6,	7,
		//
		8,	9,	11,
		9,	10,	11,
		//
		12,	13,	15,
		13,	14,	15,
		//
		16,	17,	19,
		17,	18,	19,
		//
		20,	21,	23,
		21,	22,	23,
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicat), indicat, GL_STATIC_DRAW);

	//
	DisableModel();
};



