#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "VertexLayout.h"

std::string get_file_contents(const char* filename);

class Shader {
private:
	unsigned int ID;
public:
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();
	Shader(const Shader& other);
	Shader& operator=(const Shader& other);
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

	void Activate();
	void Deactivate();
	void Delete();
	unsigned int get_id();
};

#endif
