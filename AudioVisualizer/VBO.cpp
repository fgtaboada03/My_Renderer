#include "VBO.h"

VBO::VBO() {
	glGenBuffers(1, &ID);
}

VBO::~VBO() {
	Delete();
}

unsigned int VBO::get_id() {
	return this->ID;
}

std::vector<float> VBO::get_vbo() {
	return this->vbo;
}


bool VBO::empty() {
	return this->vbo.empty();
}

int VBO::buffer_data() {
	GLint boundVBO = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundVBO);

	if (boundVBO != ID) {
		return 1;
	}

	this->vbo.shrink_to_fit();
	glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);
	return 0;
}

void VBO::copy_vector(std::vector<float> vector) {
	this->vbo = vector;
}

void VBO::append_data(float data) {
	this->vbo.push_back(data);
}

void VBO::insert_data(int index, float data) {
	this->vbo[index] = data;
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}