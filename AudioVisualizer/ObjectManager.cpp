#include "ObjectManager.h"

void ObjectManager::clear() {
	this->objects.clear();
	this->free_pool = std::queue<unsigned short>();
	count = 0;
}

void ObjectManager::delete_object(unsigned short id) {
	this->objects.erase(id);
	this->free_pool.push(id);
}

void ObjectManager::append_cords_data(unsigned short id, std::vector<GLfloat> cords_data) {
	for (GLfloat data : cords_data) {
		this->objects.at(id).cords_data.push_back(data);
	}
}

void ObjectManager::append_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba) {
	struct object obj = this->objects.at(id);

	// RGB and RGBA are mutually exclusive
	if (obj.layout.has_color() && is_rgba != obj.layout.has_rgba()) {
		return;
	}

	for (GLfloat data : color_data) {
		obj.color_data.push_back(data);
	}

	obj.layout.add_color(is_rgba);
}

void ObjectManager::append_tex_data(unsigned short id, std::vector<GLfloat> tex_data) {
	struct object obj = this->objects.at(id);
	
	for (GLfloat data : tex_data) {
		obj.tex_data.push_back(data);
	}

	obj.layout.add_uv();
}

void ObjectManager::append_indices(unsigned short id, std::vector<GLuint> indices) {
	for (GLuint data : indices) {
		this->objects.at(id).indices.push_back(data);
	}
}

void ObjectManager::replace_cords_data(unsigned short id, std::vector<GLfloat> cords_data) {
	this->objects.at(id).cords_data = cords_data;
}

void ObjectManager::replace_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba) {
	this->objects.at(id).color_data = color_data;
	this->objects.at(id).layout.add_color(is_rgba);
}

void ObjectManager::replace_tex_data(unsigned short id, std::vector<GLfloat> tex_data) {
	this->objects.at(id).tex_data = tex_data;
	this->objects.at(id).layout.add_uv();
}

void ObjectManager::replace_indices(unsigned short id, std::vector<GLuint> indices) {
	this->objects.at(id).indices = indices;
}

unsigned short ObjectManager::new_object() {
	if (!this->free_pool.empty()) {
		unsigned short id{ this->free_pool.front() };
		this->free_pool.pop();
		return id;
	}
	return ++this->count;
}

void ObjectManager::add_obj(object& obj) {
	unsigned short id{ this->new_object() };
	this->objects[id] = obj;
}

void ObjectManager::add_obj(std::vector<GLfloat> cords, std::vector<GLfloat> color, std::vector<GLfloat> tex, std::vector<GLuint> indices, bool is_rgba) {
	unsigned short id { this->new_object() };
	this->replace_cords_data(id, cords);
	this->replace_color_data(id, color, is_rgba);
	this->replace_tex_data(id, tex);
	this->replace_indices(id, indices);
}