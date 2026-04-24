#include "EBO.h"

EBO::EBO() {
	glGenBuffers(1, &ID);
}

void EBO::buffer_data() {
	this->ebo.shrink_to_fit();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->ebo.size() * sizeof(GLuint), this->ebo.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::copy_vector(std::vector<GLuint> vector) {
	this->ebo = vector;
}

void EBO::append_data(GLuint data) {
	this->ebo.push_back(data);
}

void EBO::insert_data(int index, GLuint data) {
	this->ebo[index] = data;
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}