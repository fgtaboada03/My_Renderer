#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

GLuint VAO::get_id() {
	return this->ID;
}

void VAO::LinkAttribs(VertexLayout& layout) {
	glVertexAttribPointer(layout.loc_xyz, 3, GL_FLOAT, GL_FALSE, layout.get_stride_byte(), (void*)0);

	// Link Color
	if (layout.has_color()) {
		glVertexAttribPointer(layout.loc_color, layout.color_components(), GL_FLOAT, GL_FALSE, layout.get_stride_byte(), (void*)(layout.get_offset_color_byte()));
	}

	// Link UV
	if (layout.has_uv()) {
		glVertexAttribPointer(layout.loc_uv, 2, GL_FLOAT, GL_FALSE, layout.get_stride_byte(), (void*)(layout.get_offset_uv_byte()));
	}
	VBO.Bind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}