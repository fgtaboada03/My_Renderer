#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class VBO {
private:
	GLuint ID;
	std::vector<GLfloat> vbo;

public:
	VBO();

	std::vector<GLfloat> get_vbo();

	bool empty();

	void buffer_data();
	void copy_vector(std::vector<GLfloat> vector);
	void append_data(GLfloat data);
	void insert_data(int index, GLfloat data);

	void Bind();
	void Unbind();
	void Delete();
};

#endif

