#include "MeshManager.h"

void MeshManager::clear() {
	this->meshes.clear();
	this->free_pool = std::queue<unsigned short>();
	count = 0;
}

void MeshManager::delete_mesh(unsigned short id) {
	this->meshes.erase(id);
	this->free_pool.push(id);
}

void MeshManager::append_cord_data(unsigned short id, std::vector<GLfloat> cords_data) {
	for (GLfloat data : cords_data) {
		this->meshes.at(id).cord_data.push_back(data);
	}
}

void MeshManager::append_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba) {
	struct mesh mesh = this->meshes.at(id);

	// RGB and RGBA are mutually exclusive
	if (mesh.layout.has_color() && is_rgba != mesh.layout.has_rgba()) {
		return;
	}

	for (GLfloat data : color_data) {
		mesh.color_data.push_back(data);
	}

	mesh.layout.add_color(is_rgba);
}

void MeshManager::append_tex_data(unsigned short id, std::vector<GLfloat> tex_data) {
	struct mesh mesh = this->meshes.at(id);
	
	for (GLfloat data : tex_data) {
		mesh.tex_data.push_back(data);
	}

	mesh.layout.add_uv();
}

void MeshManager::append_indices(unsigned short id, std::vector<GLuint> indices) {
	for (GLuint data : indices) {
		this->meshes.at(id).indices.push_back(data);
	}
}

void MeshManager::replace_cord_data(unsigned short id, std::vector<GLfloat> cords_data) {
	this->meshes.at(id).cord_data = cords_data;
}

void MeshManager::replace_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba) {
	this->meshes.at(id).color_data = color_data;
	this->meshes.at(id).layout.add_color(is_rgba);
}

void MeshManager::replace_tex_data(unsigned short id, std::vector<GLfloat> tex_data) {
	this->meshes.at(id).tex_data = tex_data;
	this->meshes.at(id).layout.add_uv();
}

void MeshManager::replace_indices(unsigned short id, std::vector<GLuint> indices) {
	this->meshes.at(id).indices = indices;
}

unsigned short MeshManager::new_mesh() {
	if (!this->free_pool.empty()) {
		unsigned short id{ this->free_pool.front() };
		this->free_pool.pop();
		return id;
	}
	return ++this->count;
}

void MeshManager::add_mesh(mesh& mesh) {
	unsigned short id{ this->new_mesh() };
	this->meshes[id] = mesh;
}

void MeshManager::add_mesh(std::vector<GLfloat> cords, std::vector<GLfloat> color, std::vector<GLfloat> tex, std::vector<GLuint> indices, bool is_rgba) {
	unsigned short id { this->new_mesh() };
	this->replace_cord_data(id, cords);
	this->replace_color_data(id, color, is_rgba);
	this->replace_tex_data(id, tex);
	this->replace_indices(id, indices);
}