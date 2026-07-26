#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <vector>
#include <iostream>

class VBO {
private:
	unsigned int ID;
	std::vector<float> vbo;

public:
	VBO();
	~VBO();

	unsigned int get_id();
	std::vector<float> get_vbo();

	bool empty();

	int buffer_data();
	void copy_vector(std::vector<float> vector);
	void append_data(float data);
	void insert_data(int index, float data);

	void bind();
	void unbind();
	void Delete();
};

#endif

