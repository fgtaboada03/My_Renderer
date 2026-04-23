#include "BufferBuilder.h"

int check_alignment(object obj) {
	// Test that each vector in an object has the same amount of rows
	// Success returns 0
	// Failure returns 1

	int three_per_row = 3;
	int two_per_row = 2;
	
	int vertices_alignment;
	int rgb_alignment;
	int tex_alignment;

	if (!obj.vertices.empty()) {
		vertices_alignment = obj.vertices.size() / three_per_row;
	} else {
		// Object without vertex data is an error
		return 1;
	}

	if (!obj.rgb_data.empty()) {
		rgb_alignment = obj.rgb_data.size() / three_per_row;
	} else {
		rgb_alignment = vertices_alignment;
	}

	if (!obj.tex_data.empty()) {
		tex_alignment = obj.tex_data.size() / two_per_row;
	} else {
		tex_alignment = vertices_alignment;
	}

	if (vertices_alignment == rgb_alignment && rgb_alignment == tex_alignment) {
		return 0;
	}

	return 1;
}

int get_object_interval(object obj) {
	int interval = 0;

	if (!obj.vertices.empty()) {
		interval += 3;
	}
	if (!obj.rgb_data.empty()) {
		interval += 3;
	}
	if (!obj.tex_data.empty()) {
		interval += 2;
	}

	return interval;
}

int buffer_object(Objects objects, object obj) {
	if (check_alignment(obj)) {
		return 1;
	}

	int interval = get_object_interval(obj);
	int object_vbo_size = static_cast<int>(obj.vertices.size() + obj.rgb_data.size() + obj.tex_data.size());
	
	for (int i = 0; i < object_vbo_size; i++) {

	}

	for (GLuint index : obj.indices) {
		objects.push_ebo(index);
	}

	return 0;
}

int buffer_objects(Objects objects) {
	objects.set_vertex_buffer_size(objects.get_vbo_size());
	objects.set_index_buffer_size(objects.get_ebo_size());

	int i = 0;
	int code;
	for (auto obj = objects.get_objects().begin(); obj != objects.get_objects().end(); obj++) {
		if (buffer_object(objects, objects.get(obj->first))) {
			return 1;
		}
	}
	return 0;
}

void buffer_arrays(std::vector<GLfloat> vbo, std::vector<GLuint> ebo, int process_code) {
	
}