#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>
#include <iostream>

class EBO {
	GLuint ID;
	std::vector<GLuint> ebo;

public:
	EBO();
	~EBO();

	GLuint get_id();
	std::vector<GLuint> get_ebo();
	GLuint* data();
	void buffer_data();
	void copy_vector(std::vector<GLuint> vector);
	void append_data(GLuint data);
	void insert_data(int index, GLuint data);
	
	int size() {
		return static_cast<int>(this->ebo.size());
	}

	void Bind();
	void Unbind();
	void Delete();
};

#endif
