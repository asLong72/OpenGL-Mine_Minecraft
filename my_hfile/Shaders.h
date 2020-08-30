#pragma once
#ifndef __Shaders__
#define __Shaders__

#include "../include/glad/glad.h"
#include "../include/GLFW/include/glfw3.h"
#include "iostream"
#include "fstream"
#include "strstream"

class SHADER
{
public:
	SHADER(std::string, std::string);
	SHADER();
	inline void useShaderProg() { glUseProgram(progID); };
	template<typename in_type>
	inline void set(std::string name, in_type value) { glUniform1f(glGetUniformLocation(progID, name.c_str()), (float)value); };
	inline void setMat4(std::string name, float value[]) { glUniformMatrix4fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, value); };
	inline unsigned getProgID() { return progID; };

private:
	unsigned progID;
};

#endif