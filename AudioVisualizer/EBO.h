#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>
#include <iostream>

class EBO {
	unsigned int ID;
	std::vector<unsigned int> ebo;

public:
	EBO();
	~EBO();

	unsigned int get_id();
	std::vector<unsigned int> get_ebo();
	unsigned int* data();
	int buffer_data();
	void copy_vector(std::vector<unsigned int> vector);
	void append_data(unsigned int data);
	void insert_data(int index, unsigned int data);
	
	int size() {
		return static_cast<int>(this->ebo.size());
	}

	void bind();
	void unbind();
	void Delete();
};

#endif
