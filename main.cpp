#define STB_IMAGE_IMPLEMENTATION
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "thread"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#include "my_hfile/Shaders.h"
#include "my_hfile/Camera.h"
#include "my_hfile/Game_Objects.h"
#include "my_hfile/world.h"
#include "my_hfile/callbacks.h"

void processInput(GLFWwindow* );
void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
void GLinit(GLFWwindow**, std::string);

void laodbackground();
void laodbuttom();
void DrawStartBackground(SHADER&);
void DrawOptionsBackground(SHADER&);
void DrawGaming(SHADER& s);
void texturein();
void blocksfilecheck();
void music_thread();
void camera_update();

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;
//鼠标初始上一帧位置设置为屏幕中央
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;
float pitch = 0.0f;//向量与xoz面夹角
float yaw = -90.0f;//
GLFWwindow* window_ID;

unsigned imageindices[] =
{
	0,1,3,
	0,2,3
};

unsigned VAO, VAO2, VBO, EBO, TEXTURE[3];
unsigned backgroundVAO[2], imageVAO[2], backgroundVBO[2], imageVBO[2], imageEBO[1], backgroundTEX_ID[2], imageTEX_ID[2];

bool cin = false;

float deltaTime = 0.0f;  //两帧之间的间隔时间
float lastFrame = 0.0f;  //上一帧绘制的时间


Camera c(glm::vec3(0.0f, 10.0f, 4.0f));
glm::mat4 view;
glm::mat4 model;
glm::mat4 projection;

SHADER s;
SHADER s3D;
World w;
std::string world_name;
std::fstream blockflie[25];
Blocks nowblocks(0, 0);
int a = -2, b = -2;
unsigned process = 0;
int main()
{
	//初始化glfw和glad
	GLinit(&window_ID, "GrassCube");
	std::thread music(music_thread);
	music.detach();

	s = SHADER("shaders/Shader.vs", "shaders/Shader.fs");
	s3D = SHADER("shaders/Shader3D.vs", "shaders/Shader.fs");
	texturein();
	laodbackground();
	laodbuttom();

	view = c.GetViewMatrix();
	std::thread camerathread(camera_update);
	camerathread.detach();
	float currentFrame;
	framebuffer_size_callback(window_ID, 1280, 720);
	while (!glfwWindowShouldClose(window_ID))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//processInput(window_ID);
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		switch (process)
		{
		case 0:
			s.useShaderProg();
			DrawStartBackground(s);
			break;
		case 1:
			s.useShaderProg();
			DrawOptionsBackground(s);
			break;
		case 2:
			s3D.useShaderProg();
			DrawGaming(s3D);
			break;
		default:
			break;
		}
		glfwSwapBuffers(window_ID);
		glfwPollEvents();
	}
	glfwTerminate();									//终止绘图窗口进程
	return 0;
}

void creatgame(int type = 0)
{

	//把控制台替换为OpenGL的图形界面(未完成)
	std::cerr << "输入世界名称" << std::endl;
	std::cin >> world_name;
	std::cerr << "生成超随机世界 - 扣1;" << std::endl;
	std::cerr << "生成柱随机世界 - 扣2;" << std::endl;
	std::cerr << "生成超平坦世界 - 扣3;" << std::endl;
	switch (type)
	{
	case 1:
		w = World(world_name, World_type::null);
		break;
	case 2:
		w = World(world_name, World_type::normal);
		break;
	case 3:
		w = World(world_name, World_type::flat);
		break;
	default:
		type = 0;
		break;
	}
}
void camera_update()
{
	while (true)
	{
		view = glm::lookAt(c.Position, c.Position + c.Front, c.Up);
		projection = glm::perspective(/*glm::radians(45.0f)*/45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		//s.setMat4("projection", glm::value_ptr(projection));
		//s.setMat4("view", glm::value_ptr(view));
	}
}

void blocksfilecheck()
{
	int x = -1, z = -1, i = 0;
	Blocks newblocks(nowblocks.blocks_x, nowblocks.blocks_z);
	//载入现有地图
	while (x < 2)
	{
		while (z < 2)
		{
			newblocks = Blocks(nowblocks.blocks_x + x, nowblocks.blocks_z + z);
			blockflie[i].open("Saves/" + world_name + "/" + newblocks.blocks_x_str + "  " + newblocks.blocks_z_str + ".dat", std::ios::out | std::ios::in | std::ios::binary);
			if (blockflie[i].fail())
			{
				exit(-1);
			}
			/*std::cout << newblocks.blocks_x_str + "  " + newblocks.blocks_z_str <<"."<< std::endl;*/
			i++;
			z++;
		}
		z = -1;
		x++;
	}
	x = 0, z = 0, i = 0;
	//实时生成地图部分
	//while (true)
	//{
	//	if (newblocks != nowblocks)
	//	{
	//		nowblocks = newblocks;

	//	}



	//}
}

void music_thread()
{
	PlaySound(L"calm1.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	while (!glfwWindowShouldClose(window_ID))
	{


	}
	PlaySound(NULL, NULL, SND_PURGE);
}

void texturein()
{
	glGenVertexArrays(4, imageVAO);
	glGenVertexArrays(4, backgroundVAO);
	glGenBuffers(4, backgroundVBO);
	glGenBuffers(4, imageVBO);
	glGenBuffers(4, imageEBO);
	glGenTextures(4, imageTEX_ID);
	glGenTextures(4, backgroundTEX_ID);
	float vertices[] = {
		// Back face
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
// Front face
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
// Left face
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
// Right face
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
// Bottom face
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
// Top face
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	/*
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(2, TEXTURE);
	glBindTexture(GL_TEXTURE_2D, TEXTURE[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/blocks/grass_top.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);					//释放图像的内存


	glBindTexture(GL_TEXTURE_2D, TEXTURE[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("textures/blocks/stone.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);					//释放图像的内存

	glBindTexture(GL_TEXTURE_2D, TEXTURE[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("textures/blocks/dirt.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);					//释放图像的内存

}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);

	}


	//if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	//	factor += 0.01f;
	//	if (factor > 1.0f)
	//		factor = 1.0f;
	//}

	//if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	//	factor -= 0.01f;
	//	if (factor < 0.0f)
	//		factor = 0.0f;
	//}

	//float cameraSpeed = 2.5f * deltaTime; //移动速度
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		c.ProcessKeyboard(FORWARD, deltaTime);
	//cameraPos += cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		c.ProcessKeyboard(BACKWARD, deltaTime);
	//cameraPos -= cameraSpeed * cameraFront;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		c.ProcessKeyboard(LEFT, deltaTime);
	//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		c.ProcessKeyboard(RIGHT, deltaTime);
	//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		c.ProcessKeyboard(SHIFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		c.ProcessKeyboard(SPACE, deltaTime);
	}

}

void laodbackground()
{
	float optionbackgroundvertex[] =
	{
			-1.0f,	1.0f,	0.0f,	0.0f,	0.0f,			//左上
			1.0f,	1.0f,	0.0f,	16.0f,	0.0f,			//右上
			-1.0f,	-1.0f,	0.0f,	0.0f,	9.0f,			//左下
			1.0f,	-1.0f,	0.0f,	16.0f,	9.0f,			//右下

	}, startbackgroundvertex[] =
	{
			-1.0f,	1.0f,	0.0f,	0.0f,	0.0f,			//左上
			1.0f,	1.0f,	0.0f,	1.0f,	0.0f,			//右上
			-1.0f,	-1.0f,	0.0f,	0.0f,	1.0f,			//左下
			1.0f,	-1.0f,	0.0f,	1.0f,	1.0f,			//右下

	};
	int width, height, nrChannels;
	unsigned char* data;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindVertexArray(backgroundVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBindTexture(GL_TEXTURE_2D, backgroundTEX_ID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(startbackgroundvertex), startbackgroundvertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//激活顶点相关属性
	glEnableVertexAttribArray(0);													//与着色器程序中的Location值相关联
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);													//与着色器程序中的Location值相关联
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageindices), imageindices, GL_STATIC_DRAW);
	//决定纹理缩放机制
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	data = stbi_load("textures/gui/title/background/panorama_1.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);					//释放图像的内存

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(backgroundVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBindTexture(GL_TEXTURE_2D, backgroundTEX_ID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(optionbackgroundvertex), optionbackgroundvertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);													//与着色器程序中的Location值相关联
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);													//与着色器程序中的Location值相关联
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageindices), imageindices, GL_STATIC_DRAW);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	data = stbi_load("textures/gui/options_background.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);					//释放图像的内存

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}
void laodbuttom()
{
	float buttomvertex[] = {
				-0.2f,	0.07f,	0.0f,	0.0f,	0.336f,			//左上
				0.2f,	0.07f,	0.0f,	0.782f,	0.336f,			//右上
				-0.2f,	-0.07f,	0.0f,	0.0f,	0.413f,			//左下
				0.2f,	-0.07f,	0.0f,	0.782f,	0.413f,			//右下
	};
	glBindVertexArray(imageVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, imageVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttomvertex), buttomvertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//激活顶点相关属性
	glEnableVertexAttribArray(0);													//与着色器程序中的Location值相关联
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);													//与着色器程序中的Location值相关联
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(imageindices), imageindices, GL_STATIC_DRAW);
	//纹理缓存对象

	glBindTexture(GL_TEXTURE_2D, imageTEX_ID[0]);
	//决定纹理缩放机制
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	int width, height, nrChannels;
	unsigned char* data;
	data = stbi_load("textures/gui/widgets.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);					//释放图像的内存

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void DrawStartBackground(SHADER& s)
{
	glm::mat4 trans;
	glBindVertexArray(backgroundVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBindTexture(GL_TEXTURE_2D, backgroundTEX_ID[0]);
	trans = glm::mat4(1.0f);
	s.setMat4("transform", glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(imageVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, imageVBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBindTexture(GL_TEXTURE_2D, imageTEX_ID[0]);
	trans = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.25f, 0.0f));
	s.setMat4("transform", glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void DrawOptionsBackground(SHADER& s)
{
	glm::mat4 trans;
	glBindVertexArray(backgroundVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBindTexture(GL_TEXTURE_2D, backgroundTEX_ID[1]);
	trans = glm::mat4(1.0f);
	s.setMat4("transform", glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(imageVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, imageVBO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imageEBO[0]);
	glBindTexture(GL_TEXTURE_2D, imageTEX_ID[0]);

	trans = glm::translate(glm::mat4(), glm::vec3(-0.5f, -0.25f, 0.0f));
	s.setMat4("transform", glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	trans = glm::translate(glm::mat4(), glm::vec3(0.5f, -0.25f, 0.0f));
	s.setMat4("transform", glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	trans = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.25f, 0.0f));
	s.setMat4("transform", glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void DrawGaming(SHADER& s)
{
	int x = 0, y = 0, z = 0, i = 0, j = 0, block = 0, size = sizeof(block);
	int blocksx = 0, blocksz = 0;
	s.setMat4("projection", glm::value_ptr(projection));
	s.setMat4("view", glm::value_ptr(view));
	glBindVertexArray(VAO);
	x = 0, y = 0, z = 0, i = 0, j = 0;
	a = -1;
	while (a < 2)
	{
		b = -1;
		while (b < 2)
		{
			x = a * 16, z = b * 16;
			i = 0;
			while (x < (1 + a) * 16)
			{
				z = (0 + b) * 16;
				while (z < (1 + b) * 16)
				{
					y = 0;
					while (y < 16)
					{
						/*f.seekg(std::ios::beg);
						f.seekg((x * 16 * 16 + z * 16 + y) * sizeof(block));*/
						blockflie[j].seekg((i++) * size);
						blockflie[j].read((char*)&block, size);
						//std::cerr << block << " " << j << std::endl;
						switch (block)
						{
						case 1:
							glBindTexture(GL_TEXTURE_2D, TEXTURE[0]);
							model = glm::translate(glm::mat4(), glm::vec3((float)x, (float)y, (float)z));
							s.setMat4("model", glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);

							break;
						case 2:
							glBindTexture(GL_TEXTURE_2D, TEXTURE[1]);
							model = glm::translate(glm::mat4(), glm::vec3((float)x, (float)y, (float)z));
							s.setMat4("model", glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);

							break;
						case 3:
							glBindTexture(GL_TEXTURE_2D, TEXTURE[2]);
							model = glm::translate(glm::mat4(), glm::vec3((float)x, (float)y, (float)z));
							s.setMat4("model", glm::value_ptr(model));
							glDrawArrays(GL_TRIANGLES, 0, 36);

							break;
						default:

							break;
						}
						glBindTexture(GL_TEXTURE_2D, 0);

						y++;
					}
					z++;
				}
				x++;
			}
			b++;
			j++;
		}
		a++;
	}
}

void GLinit(GLFWwindow** window_ID, std::string windowsName = std::string())
{
	//初始化glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*window_ID = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowsName.c_str(), NULL, NULL);
	while (*window_ID == NULL)
	{
		exit(-1);
	}
	glfwMakeContextCurrent(*window_ID);										//
	glfwSetFramebufferSizeCallback(*window_ID, framebuffer_size_callback);	//
	glfwSetKeyCallback(*window_ID, key_callback);
	glfwSetMouseButtonCallback(*window_ID, mouse_button_callback);
	glfwSetCursorPosCallback(*window_ID, mouse_callback);
	glfwSetInputMode(*window_ID, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{

		switch (process)
		{
		case 0:
			if (key == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(window, true);
			}
			break;
		case 1:

			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				process = 0;

				break;
			case GLFW_KEY_BACKSPACE:
				world_name.pop_back();

				break;
			default:
				if (cin)
				{
					if (key >= '0' && key <= '9' || key >= 'a' && key <= 'z' || key >= 'A' && key <= 'Z')
					{
						world_name.push_back((char)key);

					}
				}

				break;
			}
		case 2:
			if (key == GLFW_KEY_ESCAPE)
			{
				process = 0;
				for (size_t i = 0; i < 9; i++)
				{
					blockflie[i].close();
				}
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			if (key == GLFW_KEY_W)
				c.ProcessKeyboard(FORWARD, deltaTime);
			//cameraPos += cameraSpeed * cameraFront;

			if (key == GLFW_KEY_S)
				c.ProcessKeyboard(BACKWARD, deltaTime);
			//cameraPos -= cameraSpeed * cameraFront;

			if (key == GLFW_KEY_A)
				c.ProcessKeyboard(LEFT, deltaTime);
			//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);

			if (key == GLFW_KEY_D)
				c.ProcessKeyboard(RIGHT, deltaTime);
			//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
			if (key == GLFW_KEY_LEFT_SHIFT)
			{
				c.ProcessKeyboard(SHIFT, deltaTime);
			}
			if (key == GLFW_KEY_SPACE)
			{
				c.ProcessKeyboard(SPACE, deltaTime);
			}
			break;
		default:

			break;
		}
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec3 front;
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	float sensitivity = 0.05;
	switch (process)
	{
	case 0:
	case 1:
		lastX = float((xpos - WINDOW_WIDTH / 2) / WINDOW_WIDTH) * 2;
		lastY = float(0 - (ypos - WINDOW_HEIGHT / 2) / WINDOW_HEIGHT) * 2;
		std::cout << lastX << "," << lastY << std::endl;
		break;
	case 2:
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		lastX = xpos;
		lastY = ypos;

		//镜头灵敏度：sensitivity
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		c.Front = glm::normalize(front);
		break;
	default:
		break;
	}

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		switch (process)
		{
		case 0:
			if (lastX >= -0.2 && lastX <= 0.2)
			{
				if (lastY >= -0.3 && lastY <= 0.19)
				{
					process = 1;
				}
			}
			break;
		case 1:
			if (lastX >= -0.7 && lastX <= -0.3)
			{
				if (lastY >= -0.3 && lastY <= 0.19)
				{
					process = 2;
					creatgame(1);
					blocksfilecheck();
					glEnable(GL_DEPTH_TEST);
					glCullFace(GL_BACK);
					glEnable(GL_CULL_FACE);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
			}
			else if (lastX >= -0.2 && lastX <= 0.2)
			{
				if (lastY >= -0.3 && lastY <= 0.19)
				{
					process = 2;
					creatgame(2);
					blocksfilecheck();
					glEnable(GL_DEPTH_TEST);
					glCullFace(GL_BACK);
					glEnable(GL_CULL_FACE);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
			}
			else if (lastX >= 0.3 && lastX <= 0.7)
			{
				if (lastY >= -0.3 && lastY <= 0.19)
				{
					process = 2;
					creatgame(3);
					blocksfilecheck();
					glEnable(GL_DEPTH_TEST);
					glCullFace(GL_BACK);
					glEnable(GL_CULL_FACE);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
			}
			break;
		case 2:

			break;

		default:
			break;
		}
	return;
}