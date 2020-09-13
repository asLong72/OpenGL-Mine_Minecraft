#pragma once
#ifndef __SHADER__
#define __SHADER__

#include "../include/glad/glad.h"
#include "../include/GLFW/include/glfw3.h"
#include "iostream"
#include "fstream"
#include "strstream"

class Shader {
public:
	Shader();
	Shader(std::string, std::string);
	inline void useShaderProg() { glUseProgram(progID); };
	inline unsigned getProgID() { return progID; };
	inline void setMat4(std::string name, float value[]) { glUniformMatrix4fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, value); };

protected:
	unsigned progID ;

};


#endif//!__SHADER__