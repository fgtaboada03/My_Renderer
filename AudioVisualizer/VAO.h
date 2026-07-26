#ifndef VAO_CLASS_H
#define VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"
#include "VertexLayout.h"

class VAO {

private:
	GLuint ID;

public:
	VAO();

	GLuint get_id();
	int LinkAttribs(VertexLayout& layout);
	void bind();
	void unbind();
	void Delete();
};

#endif
