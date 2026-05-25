#include "VBO.h"

VBO::VBO() {
	glGenBuffers(1, &ID);
}

VBO::~VBO() {
	Delete();
}

GLuint VBO::get_id() {
	return this->ID;
}

std::vector<GLfloat> VBO::get_vbo() {
	return this->vbo;
}

GLfloat* VBO::data() {
	return vbo.data();
}

bool VBO::empty() {
	return this->vbo.empty();
}

void VBO::buffer_data() {
	this->vbo.shrink_to_fit();

	std::cout << "\nvbo:\n" << this->vbo.size() << std::endl << this->vbo.capacity() << std::endl;

	for (int i = 0; i < this->vbo.size(); i++) {
		std::cout << this->vbo.data()[i] << std::endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(GLuint), this->vbo.data(), GL_STATIC_DRAW);
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