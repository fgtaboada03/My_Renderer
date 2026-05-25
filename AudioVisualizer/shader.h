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
	GLuint ID;
public:
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	void Activate();
	void Deactivate();
	void Delete();
	GLuint get_id();
};

#endif
