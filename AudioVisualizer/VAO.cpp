#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

unsigned int VAO::get_id() {
	return this->ID;
}

int VAO::LinkAttribs(VertexLayout& layout) {
	GLint boundVAO = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);

	if (boundVAO != ID) {
		return 1;
	}

	glVertexAttribPointer(layout.loc_xyz, 3, GL_FLOAT, GL_FALSE, layout.get_stride_byte(), (void*)0);
	glEnableVertexAttribArray(layout.loc_xyz);

	// Link Color
	if (layout.has_color()) {
		glEnableVertexAttribArray(layout.loc_color);
		glVertexAttribPointer(layout.loc_color, layout.color_components(), GL_FLOAT, GL_FALSE, layout.get_stride_byte(), layout.get_color_offset_vao());
	}

	// Link UV
	if (layout.has_uv()) {
		glEnableVertexAttribArray(layout.loc_uv);
		glVertexAttribPointer(layout.loc_uv, 2, GL_FLOAT, GL_FALSE, layout.get_stride_byte(), layout.get_uv_offset_vao());
	}
	return 0;
}

void VAO::bind() {
	glBindVertexArray(ID);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}