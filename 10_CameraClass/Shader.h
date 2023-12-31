#pragma once
#pragma once

#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


class Shader {
public:
	unsigned int ID;				// program ID

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif