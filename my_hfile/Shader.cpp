#include "Shader.h"

Shader::Shader() {
	progID = 0;
}

Shader::Shader(std::string vertexPath_in, std::string fragmentPath_in) {
	//1
	//从文件中读取顶点着色器代码，并经字符串流存入字符串中
	std::string vertexCode_str;
	std::ifstream vertexCode_fstream;
	std::strstream vertexCode_strstream;

	try
	{
		vertexCode_fstream.open(vertexPath_in);
		vertexCode_strstream << vertexCode_fstream.rdbuf() << std::ends;
		vertexCode_str = vertexCode_strstream.str();
		vertexCode_fstream.close();
	}
	catch (const std::exception&)
	{
		std::cerr << vertexPath_in << " error!" << std::endl;;
	}

	//从文件中读取片元着色器代码，并经字符串流存入字符串中
	std::string fragmentCode_str;
	std::ifstream fragmentCode_fstream;
	std::strstream fragmentCode_strstream;

	try
	{
		fragmentCode_fstream.open(fragmentPath_in);
		fragmentCode_strstream << fragmentCode_fstream.rdbuf() << std::ends;
		fragmentCode_str = fragmentCode_strstream.str();
		fragmentCode_fstream.close();
	}
	catch (const std::exception&)
	{
		std::cerr << fragmentPath_in << " error!" << std::endl;;
	}

	//2
	int success;
	char infolog[512];
	//编译顶点着色器
	const char* vertexCode_c_str = vertexCode_str.c_str();
	unsigned vertex_ID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_ID, 1, &vertexCode_c_str, NULL);
	glCompileShader(vertex_ID);
	glGetShaderiv(vertex_ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_ID, sizeof(infolog), NULL, infolog);
		std::cerr << "GL_VERTEX_SHADER:" << infolog << std::endl;
	}
	//编译片元着色器
	const char* fragmentCode_c_str = fragmentCode_str.c_str();
	unsigned fragment_ID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_ID, 1, &fragmentCode_c_str, NULL);
	glCompileShader(fragment_ID);
	glGetShaderiv(fragment_ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_ID, sizeof(infolog), NULL, infolog);
		std::cerr << "GL_FRAGMENT_SHADER:" << infolog << std::endl;
	}
	//链接着色器程序
	progID = glCreateProgram();
	glAttachShader(progID, vertex_ID);
	glAttachShader(progID, fragment_ID);
	glLinkProgram(progID);
	glGetProgramiv(progID, GL_LINK_STATUS, &success);			//检查着色器程序是否链接成功
	if (!success)
	{
		glGetProgramInfoLog(progID, 512, NULL, infolog);
		std::cerr << "glLinkProgram:" << infolog << std::endl;
	}
	//删除着色器程序
	glDeleteShader(vertex_ID);
	glDeleteShader(fragment_ID);
}