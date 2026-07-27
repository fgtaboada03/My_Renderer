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


	glEnableVertexAttribArray(layout.loc_xyz);
	glVertexAttribPointer(layout.loc_xyz, 3, GL_FLOAT, GL_FALSE, layout.get_stride_byte(), layout.get_xyz_offset_vao());


	// Link Color
	if (layout.has_color()) {
		std::cout << "color offset bytes: " << (intptr_t)layout.get_color_offset_vao() << std::endl;
		glVertexAttribPointer(layout.loc_color, layout.color_components(), GL_FLOAT, GL_FALSE, layout.get_stride_byte(), layout.get_color_offset_vao());
		glEnableVertexAttribArray(layout.loc_color);

		for (GLenum err; (err = glGetError()) != GL_NO_ERROR; ) {
			std::cout << "GL error after LinkAttribs: 0x" << std::hex << err << std::endl;
		}
	}

	// Link UV
	if (layout.has_uv()) {
		std::cout << "texture offset bytes: " << (intptr_t)layout.get_uv_offset_vao() << std::endl;

		glEnableVertexAttribArray(layout.loc_uv);
		glVertexAttribPointer(layout.loc_uv, 2, GL_FLOAT, GL_FALSE, layout.get_stride_byte(), layout.get_uv_offset_vao());

		for (GLenum err; (err = glGetError()) != GL_NO_ERROR; ) {
			std::cout << "GL error after LinkAttribs: 0x" << std::hex << err << std::endl;
		}
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