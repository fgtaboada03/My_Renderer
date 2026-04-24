#include "VBO.h"

VBO::VBO() {
	glGenBuffers(1, &ID);
}

std::vector<GLfloat> VBO::get_vbo() {
	return this->vbo;
}


bool VBO::empty() {
	return this->vbo.empty();
}

void VBO::buffer_data() {
	this->vbo.shrink_to_fit();
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, this->vbo.size() * sizeof(GLfloat), this->vbo.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::copy_vector(std::vector<GLfloat> vector) {
	this->vbo = vector;
}

void VBO::append_data(GLfloat data) {
	this->vbo.push_back(data);
}

void VBO::insert_data(int index, GLfloat data) {
	this->vbo[index] = data;
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}