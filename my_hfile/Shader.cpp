#include "Shader.h"

Shader::Shader() {
	progID = 0;
}

Shader::Shader(std::string vertexPath_in, std::string fragmentPath_in) {
	//1
	//���ļ��ж�ȡ������ɫ�����룬�����ַ����������ַ�����
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

	//���ļ��ж�ȡƬԪ��ɫ�����룬�����ַ����������ַ�����
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
	//���붥����ɫ��
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
	//����ƬԪ��ɫ��
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
	//������ɫ������
	progID = glCreateProgram();
	glAttachShader(progID, vertex_ID);
	glAttachShader(progID, fragment_ID);
	glLinkProgram(progID);
	glGetProgramiv(progID, GL_LINK_STATUS, &success);			//�����ɫ�������Ƿ����ӳɹ�
	if (!success)
	{
		glGetProgramInfoLog(progID, 512, NULL, infolog);
		std::cerr << "glLinkProgram:" << infolog << std::endl;
	}
	//ɾ����ɫ������
	glDeleteShader(vertex_ID);
	glDeleteShader(fragment_ID);
}