#include <Windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

#include "../glad/glad.h"
#include "../GLFW/include/glfw3.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "my_hfile/Camera.h"
#include "my_hfile/Shader.h"
#include "my_hfile/CubeModel.h"
#include "my_hfile/TextureBuffer.h"
#include "my_hfile/Chunk.h"
#include "my_hfile/Noise.h"


void GLinti(GLFWwindow*& window);
void FramebufferSize_callback(GLFWwindow*, int, int);
void Key_callback(GLFWwindow*, int, int, int, int);
void MouseButton_callback(GLFWwindow*, int, int, int);
void MousePosition_callback(GLFWwindow*, double, double);
void processInput(GLFWwindow*);


#define WIN_WIDTH 1080
#define WIN_HEIGHT 720
Camera c;
CubeModel m;
Blocks b;
float deltatime;
float lastX, lastY;

int main()
{
	GLFWwindow* window;
	GLinti(window);
	//m = CubeModel("Shaders/CubeShader/CubeFS.glsl", "Shaders/CubeShader/CubeFS.glsl");

	b = Blocks("Shaders/CubeShader/CubeInstancedVS.glsl", "Shaders/CubeShader/CubeInstancedFS.glsl");
	short x = 0, y = 0, z = 0, chunkX = 0, chunkY = 0, height = 0;
	unsigned char a[16 * 16 * 16] = { 0 };
	while (x < 16)
	{
		while (z < 16)
		{
			height = (short)(PerlinNoise(x, z) * 100.0) % 17;
			while (y < 16)
			{
				switch (y)
				{
				case 12:
					a[x * 16 * 16 + z * 16 + y] = 3;
					break;
				case 13:
					a[x * 16 * 16 + z * 16 + y] = 3;
					break;
				case 14:
					a[x * 16 * 16 + z * 16 + y] = 2;
					break;
				case 15:
					a[x * 16 * 16 + z * 16 + y] = 1;
					break;
				default:
					a[x * 16 * 16 + z * 16 + y] = 4;
					break;
				}
				y++;
			}
			z++, y = 0;
		}
		x++, z = 0;
	}
	x = 0, y = 0, z = 0;
	b.Init(a);
	c = Camera(glm::vec3(0, 0, 20));
	//TextureBuffer t("textures/blocks/grass_top.png");
	std::vector<std::string> path;
	path.push_back("textures/blocks/grass_top.png");
	path.push_back("textures/blocks/dirt.png");
	path.push_back("textures/blocks/gravel.png");
	path.push_back("textures/blocks/stone.png");
	path.push_back("textures/blocks/brick.png");
	path.push_back("textures/blocks/clay.png");
	TextureBuffer t2(path);

	glm::mat4 model(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f)))
		, view = c.GetViewMatrix()
		, projection = glm::perspective(/*glm::radians(45.0f)*/45.0f, (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);

	float now, lastframetime;
	lastframetime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);
		
		b.cube_shader.useShaderProg();
		b.UsingModel();
		t2.Enabletexture();
		//m.setMat4("model", glm::value_ptr(model));
		view = c.GetViewMatrix();
		b.cube_shader.setMat4("view", glm::value_ptr(view));
		b.cube_shader.setMat4("projection", glm::value_ptr(projection));
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL, b.getNum());

		t2.Disabletexture();

		glfwSwapBuffers(window);
		now = glfwGetTime();
		deltatime = now - lastframetime;
		lastframetime = now;
		//std::cerr << 1.0f / deltatime << std::endl;
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
void GLinti(GLFWwindow*& window)
{
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "test", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "window" << std::endl;
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, Key_callback);
	glfwSetCursorPosCallback(window, MousePosition_callback);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);

	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		c.ProcessKeyboard(FORWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		c.ProcessKeyboard(BACKWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		c.ProcessKeyboard(LEFT, deltatime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		c.ProcessKeyboard(RIGHT, deltatime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		c.ProcessKeyboard(SHIFT, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		c.ProcessKeyboard(SPACE, deltatime);
	}

}
void FramebufferSize_callback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
}
void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			c.ProcessKeyboard(FORWARD, deltatime);
			break;
		case GLFW_KEY_S:
			c.ProcessKeyboard(BACKWARD, deltatime);
			break;
		case GLFW_KEY_A:
			c.ProcessKeyboard(LEFT, deltatime);
			break;
		case GLFW_KEY_D:
			c.ProcessKeyboard(RIGHT, deltatime);
			break;
		default:
			break;
		}
	}
}
void MouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{

}
void MousePosition_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset = lastX - xpos;	//别忘了，在窗口中，左边的坐标小于右边的坐标，而我们需要一个正的角度
	float yoffset = lastY - ypos;	//同样，在窗口中，下面的坐标大于上面的坐标，而我们往上抬头的时候需要一个正的角度
	lastX = xpos;
	lastY = ypos;
	c.ProcessMouseMovement(xoffset, yoffset);
}