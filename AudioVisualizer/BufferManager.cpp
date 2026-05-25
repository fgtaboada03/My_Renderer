#include "BufferManager.h"

GLuint MeshBuffers::get_vbo_id() {
	return this->vbo.get_id();
}

std::vector<GLfloat> MeshBuffers::get_vbo() {
	return this->vbo.get_vbo();
}

GLuint MeshBuffers::get_ebo_id() {
	return this->ebo.get_id();
}

std::vector<GLuint> MeshBuffers::get_ebo() {
	return this->ebo.get_ebo();
}

GLuint MeshBuffers::get_vao_id() {
	return this->vao.get_id();
}

void MeshBuffers::erase() {
	this->vbo.Delete();
	this->ebo.Delete();
	this->vao.Delete();
}

bool MeshBuffers::empty() {
	return this->vbo.empty();
}

void MeshBuffers::append_vbo(GLfloat data) {
	this->vbo.append_data(data);
}

void MeshBuffers::append_ebo(GLuint data) {
	this->ebo.append_data(data);
}

void MeshBuffers::buffer_data() {
	this->vbo.buffer_data();
	this->ebo.buffer_data();
}

void MeshBuffers::bind() {
	this->vao.Bind();
	this->vbo.Bind();
	this->ebo.Bind();
}

void MeshBuffers::unbind() {
	this->vbo.Unbind();
	this->ebo.Unbind();
	this->vao.Unbind();
}

void MeshBuffers::EnableAttribs() {
	glEnableVertexAttribArray(layout.loc_xyz);

	if (this->layout.has_color()) {
		glEnableVertexAttribArray(layout.loc_color);
	}
	if (this->layout.has_uv()) {
		glEnableVertexAttribArray(layout.loc_uv);
	}
}

void MeshBuffers::DisableAttribs() {
	glDisableVertexAttribArray(layout.loc_xyz);

	if (this->layout.has_color()) {
		glDisableVertexAttribArray(layout.loc_color);
	}
	if (this->layout.has_uv()) {
		glDisableVertexAttribArray(layout.loc_uv);
	}
}

void MeshBuffers::draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>& shader_table) {
	Shader& shader = shader_table.at(layout.flags);

	GLboolean doesVAOExist = static_cast<bool>(glIsVertexArray(vao.get_id()));

	if (GL_FALSE == doesVAOExist) {
		std::cout << "false" << std::endl;
	}
	else if (GL_TRUE == doesVAOExist) {
		std::cout << "true" << std::endl;
	}

	//GLfloat* data;

	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vbo.data()), data);

	//for (int i = 0; i < sizeof(data) / sizeof(GLfloat); i++) {
	//	std::cout << "i: " << data[i] << std::endl;
	//}

	shader.Activate();
	cam.Inputs(window);
	cam.Matrix(fov, near, far, shader, "camMatrix");
	this->bind();
	this->EnableAttribs();

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->get_ebo().size()), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->get_ebo().size()));

	this->DisableAttribs();
	this->unbind();
	shader.Deactivate();
}

BufferManager::~BufferManager() {
	for (auto it = meshes.begin(); it != meshes.end(); it++) {
		it->second.erase();
	}
}

std::unordered_map<uint32_t, MeshBuffers> BufferManager::get() {
	return this->meshes;
}

void BufferManager::clear() {
	this->meshes.clear();
}

void BufferManager::commit(std::unordered_map<unsigned short, object>& objects) {
	int offset = 0;
	
	// Interweaving Buffer Data
	for (auto& it : objects) {
		object& obj = it.second;
		VertexLayout layout = obj.layout;
		MeshBuffers& mesh_buffer = meshes[layout.flags];

		mesh_buffer.layout = layout;

		int i, partition, n_strides, vector_index, stride_local_index;

		for (i = 0; i < obj.vbo_size(); i++) {
			stride_local_index = i % layout.stride;
			partition = layout.getPositionInPartition(i % layout.stride);
			n_strides = i / layout.stride;

			if (partition == 0) {
				vector_index = (n_strides * 3) + stride_local_index;
				GLfloat data = obj.cords_data[vector_index];
				mesh_buffer.append_vbo(data);
			}
			else if (partition == 1) {
				vector_index = (n_strides * layout.color_components()) + stride_local_index - layout.offset_color;
				GLfloat data = obj.color_data[vector_index];
				mesh_buffer.append_vbo(data);
			}
			else if (partition == 2) {
				vector_index = (n_strides * 2) + stride_local_index - layout.offset_uv;
				GLfloat data = obj.tex_data[vector_index];
				mesh_buffer.append_vbo(data);
			}
		}

		// Consider Object Offset of indices.
		// Each Mesh has it's local index but not
		// it's global buffer index.
		for (GLuint data : obj.indices) {
			mesh_buffer.append_ebo(data);
		}

		offset += static_cast<int>(obj.vbo_size()) / layout.stride;
	}

	// Sending to 
	for (auto& mesh : meshes) {
		MeshBuffers& mesh_buffer = mesh.second;
		VertexLayout& layout = mesh.second.layout;

		if (mesh_buffer.empty()) {
			continue;
		}

		mesh_buffer.vbo.Bind();
		mesh_buffer.vao.Bind();
		mesh_buffer.buffer_data();

		// Link XYZ
		mesh_buffer.vao.LinkAttribs(mesh_buffer.vbo, layout);
		mesh_buffer.unbind();
	}

	//for (auto mesh : meshes) {
	//	MeshBuffers& mesh_buffer = mesh.second;
	//	VertexLayout layout = mesh.second.layout;

	//	std::cout << "vbo id: " << mesh_buffer.get_vbo_id() << std::endl;

	//	for (auto data : mesh_buffer.get_vbo()) {
	//		std::cout << data << std::endl;
	//	}

	//	std::cout << "ebo id: " << mesh_buffer.get_ebo_id() << std::endl;

	//	for (auto data : mesh_buffer.get_ebo()) {
	//		std::cout << data << std::endl;
	//	}

	//	std::cout << "vao id: " << mesh_buffer.get_vao_id() << std::endl;
	//}
}

void BufferManager::draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>& shader_table) {
	for (auto& it : meshes) {
		it.second.draw(cam, window, fov, near, far, shader_table);
	}
}