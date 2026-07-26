#include "EBO.h"

EBO::EBO() {
	glGenBuffers(1, &ID);
}

EBO::~EBO() {
	Delete();
}

unsigned int EBO::get_id() {
	return this->ID;
}

std::vector<unsigned int> EBO::get_ebo() {
	return this->ebo;
}

unsigned int* EBO::data() {
	return ebo.data();
}

int EBO::buffer_data() {
	GLint boundEBO = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundEBO);

	if (boundEBO != ID) {
		return 1;
	}

	this->ebo.shrink_to_fit();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.size() * sizeof(unsigned int), this->ebo.data(), GL_STATIC_DRAW);
	return 0;
}

void EBO::copy_vector(std::vector<unsigned int> vector) {
	this->ebo = vector;
}

void EBO::append_data(unsigned int data) {
	this->ebo.push_back(data);
}

void EBO::insert_data(int index, unsigned int data) {
	this->ebo[index] = data;
}

void EBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}