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

    Shader(const Shader& other) {
        std::cout << "Copy Constructor " << ID << " << " << other.ID << std::endl;
        ID = other.ID;
    }

    Shader& operator=(const Shader& other) {
        std::cout << "Copy Constructor = " << ID << " << " << other.ID << std::endl;
        ID = other.ID;
    }

    Shader(Shader&& other) noexcept
        : ID(other.ID)
    {
        std::cout << "Move Constructor " << ID << std::endl;
        other.ID = 0;
    }

    Shader& operator=(Shader&& other) noexcept
    {
        std::cout << "Move Constructor = " << ID << std::endl;
        if (this != &other)
        {
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }

	void Activate();
	void Deactivate();
	void Delete();
	unsigned int get_id();
};

#endif
