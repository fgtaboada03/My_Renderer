#include "objects.h"

void Objects::clear() {
	this->clear_objects();
	this->clear_buffers();
}

void Objects::clear_objects() {
	this->objects.clear();
	count = 0;
}

void Objects::clear_buffers() {
	this->vbo.clear();
	this->ebo.clear();
}

struct object Objects::get(unsigned short id) {
	return this->objects.at(id);
}

std::unordered_map<unsigned short, object> Objects::get_objects() {
	return this->objects;
}

std::vector<GLfloat> Objects::get_vbo_vector() {
	return this->vbo;
}

std::vector<GLuint> Objects::get_ebo_vector() {
	return this->ebo;
}

GLfloat* Objects::get_vbo_array() {
	return this->vbo.data();
}

GLuint* Objects::get_ebo_array() {
	return this->ebo.data();
}

int Objects::get_byte_size_vertices() {
	return static_cast<int>(this->vbo.size() * sizeof(GLfloat));
}

int Objects::get_byte_size_indices() {
	return static_cast<int>(this->ebo.size() * sizeof(GLuint));
}

int Objects::get_vbo_size() {
	return this->vbo_size;
}

int Objects::get_ebo_size() {
	return this->ebo_size;
}

void Objects::push_vbo(GLfloat point) {
	if (point <= 1.0 && point >= -1.0) {
		this->vbo.push_back(point);
	}
}

void Objects::push_ebo(GLuint index) {
	this->ebo.push_back(index);
}

void Objects::set_vertex_buffer_size(int size) {
	this->vbo.reserve(size);
}

void Objects::set_index_buffer_size(int size) {
	this->ebo.reserve(size);
}

void Objects::delete_object(unsigned short id) {
	if (objects.contains(id)) {
		objects.erase(id);
		free_pool.push(id);
	}
}

unsigned short Objects::get_new_id() {
	return ++count;
}

void Objects::add_vertices(unsigned short id, std::vector<GLfloat> vertices) {
	this->objects[id].vertices = vertices;
	this->vbo_size += static_cast<int>(vertices.size());
}

void Objects::add_rgb_data(unsigned short id, std::vector<GLfloat> rgb_data, bool rgba) {
	this->objects[id].rgb_data = rgb_data;
	this->vbo_size += static_cast<int>(rgb_data.size());
	this->objects[id].rgba = rgba;
}

void Objects::add_tex_data(unsigned short id, std::vector<GLfloat> tex_data) {
	this->objects[id].tex_data = tex_data;
	this->vbo_size += static_cast<int>(tex_data.size());
}

void Objects::add_indices(unsigned short id, std::vector<GLuint> indices) {
	this->objects[id].indices = indices;
	this->ebo_size += static_cast<int>(indices.size());
}

void Objects::add_obj(std::vector<GLfloat> vertices, std::vector<GLfloat> rgb_data, std::vector<GLfloat> tex_data, std::vector<GLuint> indices) {
	int id = get_new_id();
	add_vertices(id, vertices);
	add_rgb_data(id, rgb_data);
	add_tex_data(id, tex_data);
	add_indices(id, indices);
}

void Objects::add_triangle(int scale, float offsetX, float offsetY, float offsetZ) {
	int id = get_new_id();

	std::vector<GLfloat> vertices = {
		( 0.5f * scale) + offsetX,  (-0.5f * scale) + offsetY,   (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX,  (-0.5f * scale) + offsetY,   (0.5f * scale) + offsetZ,
		( 0.0f * scale) + offsetX,  ( 0.5f * scale) + offsetY,   (0.5f * scale) + offsetZ
	};

	std::vector<GLfloat> rgb_data = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	std::vector<GLfloat> tex_data = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	std::vector<GLuint> indices = {
		0, 1, 2
	};

	add_vertices(id, vertices);
	add_rgb_data(id, rgb_data);
	add_tex_data(id, tex_data);
	add_indices(id, indices);
}

void Objects::add_square(int scale, float offsetX, float offsetY, float offsetZ) {
	int id = get_new_id();

	std::vector<GLfloat> vertices = {
		( 0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		( 0.5f * scale) + offsetX, ( 0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, ( 0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ
	};

	std::vector<GLfloat> rgb_data = {
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	std::vector<GLfloat> tex_data = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	std::vector<GLuint> indices = {
		0, 1, 2,
		3, 2, 1
	};

	add_vertices(id, vertices);
	add_rgb_data(id, rgb_data);
	//add_tex_data(id, tex_data);
	add_indices(id, indices);
}

void Objects::add_cube(int scale, float offsetX, float offsetY, float offsetZ) {
	int id = get_new_id();

	std::vector<GLfloat> vertices = {
		( 0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  ( 0.5f * scale) + offsetZ,
		( 0.5f * scale) + offsetX, ( 0.5f * scale) + offsetY,  ( 0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  ( 0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, ( 0.5f * scale) + offsetY,  ( 0.5f * scale) + offsetZ,
		( 0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ,
		( 0.5f * scale) + offsetX, ( 0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, ( 0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ
	};
	std::vector<GLfloat> rgb_data = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	std::vector<GLfloat> tex_data = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};
	std::vector<GLuint> indices = {
		0, 1, 2,
		3, 2, 1,
		4, 5, 6,
		7, 5, 6,
		4, 5, 0,
		1, 0, 5,
		6, 7, 2,
		3, 2, 7,
		1, 3, 5,
		7, 5, 3,
		0, 2, 4,
		6, 4, 2
	};

	add_vertices(id, vertices);
	add_rgb_data(id, rgb_data);
	//add_tex_data(id, tex_data);
	add_indices(id, indices);
}