#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO {
	GLuint ID;
	std::vector<GLuint> ebo;

public:
	EBO();

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
