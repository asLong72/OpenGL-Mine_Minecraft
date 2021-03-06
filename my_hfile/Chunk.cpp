#include "Chunk.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include <vector>

Blocks::Blocks() {
	AllData = NULL;
	inNeedData = NULL;
	PosintancedVBO = 0;
	TexintancedVBO = 0;
	intance_num = 0;
	visualable = false;
}
Blocks::Blocks(std::string v_path, std::string f_path) :CubeModel(v_path, f_path) {
	AllData = NULL;
	inNeedData = NULL;
	PosintancedVBO = 0;
	TexintancedVBO = 0;
	intance_num = 0;
	visualable = false;
}
void Blocks::Init(unsigned char* data_in) {
	AllData = data_in;
	visualable = true;
	inNeedData_Update();
	setIntancedVBO();
}
void Blocks::inNeedData_Update() {
	short x = 0, y = 0, z = 0, pos = 0;
	inNeedData = new unsigned char[16 * 16 * 16];
	Block now = {
		now.up = 0,
		now.down = 0,
		now.right = 0,
		now.left = 0,
		now.front = 0,
		now.behind = 0,
	};
	while (x < 16)
	{
		while (z < 16)
		{
			while (y < 16)
			{
				now.up = 0;
				now.down = 0;
				now.right = 0;
				now.left = 0;
				now.front = 0;
				now.behind = 0;
				pos = 16 * 16 * x + 16 * z + y;

				if (x != 0)
				{
					now.left = AllData[pos - 16 * 16];
				}
				if (x != 15)
				{
					now.right = AllData[pos + 16 * 16];
				}

				if (y != 0)
				{
					now.down = AllData[pos - 16];
				}
				if (y != 15)
				{
					now.up = AllData[pos + 16];
				}

				if (z != 0)
				{
					now.behind = AllData[pos - 1];
				}
				if (z != 15)
				{
					now.front = AllData[pos + 1];
				}

				if (now.up == 0 || now.down == 0 || now.behind == 0 || now.front == 0 || now.left == 0 || now.right == 0)
				{
					inNeedData[pos] = AllData[pos];
				}
				else
				{
					inNeedData[pos] = 0;
				}

				y++;
			}
			z++, y = 0;
		}
		x++, z = 0;
	}
}
//각겔각것각것각것
void Blocks::setIntancedVBO() {
	short x = 0, y = 0, z = 0;
	int pos = 0;
	glm::mat4 need;
	std::vector<glm::mat4> intancedmv;
	std::vector<GLfloat> intancedtex;
	while (x < 16)
	{
		while (z < 16)
		{
			while (y < 16)
			{
				pos = 16 * 16 * x + 16 * z + y;
				if (inNeedData[pos] != 0)
				{
					intancedmv.push_back(glm::translate(glm::mat4(), glm::vec3(x, y, z)));
					intancedtex.push_back((float)(inNeedData[pos] - 1));
					intance_num++;
				}
				y++;
			}
			z++, y = 0;
		}
		x++, z = 0;
	}
	std::cerr << intance_num << std::endl;
	std::cerr << intancedtex.size() << std::endl;
	glBindVertexArray(VAO);
	glGenBuffers(1, &PosintancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PosintancedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * intance_num, intancedmv.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, glm::vec4().length(), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, glm::vec4().length(), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, glm::vec4().length(), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, glm::vec4().length(), GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	glGenBuffers(1, &TexintancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TexintancedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * intance_num, intancedtex.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(NULL);
}