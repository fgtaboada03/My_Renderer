#include "BufferBuilder.h"

int check_alignment(object obj) {
	// Test that each vector in an object has the same amount of rows
	// Success returns 0
	// Failure returns 1

	int four_per_row = 4;
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
		if (!obj.rgba) {
			rgb_alignment = obj.rgb_data.size() / three_per_row;
		}
		else {
			rgb_alignment = obj.rgb_data.size() / four_per_row;
		}
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
	if (!obj.rgb_data.empty() && !obj.rgba) {
		if (obj.rgba) {
			interval += 3;
		} else {
			interval += 4;
		}
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
	
	int local_location, n_scanned_vertices, local_index, i, offset;
	for (i = 0; i < object_vbo_size; i++) {
		local_location = object_vbo_size % interval;
		n_scanned_vertices = i / interval;

		if (local_location >= 0 && local_location <= 2) {
			// xyz
			local_index = (n_scanned_vertices * 3) + local_location;
			objects.push_vbo(obj.vertices[local_index]);
		}
		else if (!obj.rgb_data.empty() && (local_location == 3 || local_location == 5 || (obj.rgba && local_location == 6))) {
			// rgb or rgba
			offset = local_location - 3;
			local_index = (n_scanned_vertices * 3 + obj.rgba) + offset;
			objects.push_vbo(obj.vertices[local_index]);
		}
		else if (!obj.tex_data.empty() && (local_location == 6 || local_location == 7 || (obj.rgba && local_location == 8))) {
			// uv
			offset = local_location + obj.rgba - 6;
			local_index = (n_scanned_vertices * 2) + offset;
			objects.push_vbo(obj.tex_data[local_index]);
		}
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