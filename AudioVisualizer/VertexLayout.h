#ifndef VERTEXLAYOUT_H
#define VERTEXLAYOUT_H

#include <glad/glad.h>
#include <cassert>
#include <vector>

// ------------------------------------------------------------
//  Attrib flags  (combine with |)
// ------------------------------------------------------------
enum VertexFlags : uint32_t {
	VERTEX_XYZ = 1 << 0,   // float x, y, z          (always required)
	VERTEX_RGB = 1 << 1,   // float r, g, b
	VERTEX_RGBA = 1 << 2,   // float r, g, b, a
	VERTEX_UV = 1 << 3,   // float u, v
};

// ------------------------------------------------------------
//  Vertex layout  — computed once from flags
// ------------------------------------------------------------
struct VertexLayout {
	uint32_t flags;

	// offsets within one vertex
	int offset_xyz = 0;
	int offset_color = 0;   // rgb or rgba
	int offset_uv = 0;

	int stride = 0;

	// attribute locations (matches the shader locations below)
	int loc_xyz = 0;
	int loc_color = -1;
	int loc_uv = -1;

	bool has_rgb()  const { return (flags & VERTEX_RGB) != 0; }
	bool has_rgba() const { return (flags & VERTEX_RGBA) != 0; }
	bool has_color() const { return has_rgb() || has_rgba(); }
	bool has_uv()   const { return (flags & VERTEX_UV) != 0; }
	int getPositionInPartition(int index) {
		if (index < 0 || index > stride) {
			return -1;
		}

		if (index >= 0 && index < 3) {
			return 0;
		}
		else if (has_color() && index >= offset_color && index < offset_color + color_components()) {
			return 1;
		}
		else if (has_uv() && index >= offset_uv && index < offset_uv + 2) {
			return 2;
		}

		return -1;
	}
	int get_offset_uv_byte() { return offset_uv * sizeof(GLfloat); }
	int get_offset_color_byte() { return offset_color * sizeof(GLfloat); }
	int get_offset_xyz_byte() { return offset_xyz * sizeof(GLfloat); }
	int get_stride_byte() {
		return stride * sizeof(GLfloat);
	}
	int color_components() const { return has_rgba() ? 4 : 3; }
	void calculate_state() {
		offset_xyz = 0;
		int cursor = 3;

		if (has_color()) {
			offset_color = cursor;
			cursor += color_components();
			loc_color = 1;
		}

		if (has_uv()) {
			offset_uv = cursor;
			cursor += 2;
			loc_uv = 2;
		}

		stride = cursor;
	}
	void add_color(bool is_rgba) {
		if (is_rgba && !has_rgba()) {
			flags = (flags ^ VERTEX_RGB) | VERTEX_RGBA;
		}
		else if (!is_rgba && !has_rgb()) {
			flags = (flags ^ VERTEX_RGBA) | VERTEX_RGB;
		}
		else {
			return;
		}

		calculate_state();
	}
	void add_uv() {
		if (has_uv()) {
			return;
		}

		flags = flags | VERTEX_UV;
		calculate_state();
	}
	void delete_color() {
		if (has_color()) {
			stride -= color_components() * sizeof(GLfloat);
			flags = flags ^ VERTEX_RGBA ^ VERTEX_RGB;
			loc_color = -1;

			if (has_uv()) {
				offset_uv = 3;
				loc_uv = 2;
			}
		}
	}
	void delete_texture() {
		if (has_uv()) {
			stride -= 2;
			flags = flags ^ VERTEX_UV;
			offset_uv = 0;
			loc_uv = -1;
		}
	}

	explicit VertexLayout(uint32_t f = VERTEX_XYZ) : flags(f) {
		// RGB and RGBA are mutually exclusive
		assert(!has_rgb() || !has_rgba() &&
			"Use VERTEX_RGB or VERTEX_RGBA, not both");

		calculate_state();
	}
	~VertexLayout() = default;
	VertexLayout(const VertexLayout& other) : flags(other.flags) {
		// RGB and RGBA are mutually exclusive
		assert(!has_rgb() || !has_rgba() &&
			"Use VERTEX_RGB or VERTEX_RGBA, not both");

		calculate_state();
	}
	VertexLayout& operator=(const VertexLayout& other) {
		if (this != &other) {
			flags = other.flags;
		}
		// RGB and RGBA are mutually exclusive
		assert(!has_rgb() || !has_rgba() &&
			"Use VERTEX_RGB or VERTEX_RGBA, not both");

		calculate_state();

		return *this;
	}

};

// ------------------------------------------------------------
//  Object — holds object data
// ------------------------------------------------------------
struct object {
	struct VertexLayout layout;

	std::vector<GLfloat> cords_data;
	std::vector<GLfloat> color_data;
	std::vector<GLfloat> tex_data;
	std::vector<GLuint>  indices;

	size_t vbo_size() { return cords_data.size() + color_data.size() + tex_data.size(); }
	size_t ebo_size() { return indices.size(); }

	object() = default;
	object(uint32_t flag) : layout(flag) {}
	object(VertexLayout layout) : layout(layout) {}
	object(uint32_t flag, std::vector<GLfloat> cords_data, std::vector<GLuint> indices, std::vector<GLfloat> color_data = {}, std::vector<GLfloat> tex_data = {}) : layout(flag) {
		this->cords_data = cords_data;
		this->indices = indices;

		if (this->layout.has_color()) {
			this->color_data = color_data;
		}
		if (this->layout.has_uv()) {
			this->tex_data = tex_data;
		}
	}
	object(VertexLayout layout, std::vector<GLfloat> cords_data, std::vector<GLuint> indices, std::vector<GLfloat> color_data = {}, std::vector<GLfloat> tex_data = {}) : layout(layout) {
		this->cords_data = cords_data;
		this->indices = indices;

		if (this->layout.has_color()) {
			this->color_data = color_data;
		}
		if (this->layout.has_uv()) {
			this->tex_data = tex_data;
		}
	}
};

#endif