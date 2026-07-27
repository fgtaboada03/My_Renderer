#include "BufferManager.h"

GLuint MeshBuffer::get_vbo_id() {
	return this->vbo.get_id();
}

std::vector<GLfloat> MeshBuffer::get_vbo() {
	return this->vbo.get_vbo();
}

GLuint MeshBuffer::get_ebo_id() {
	return this->ebo.get_id();
}

std::vector<GLuint> MeshBuffer::get_ebo() {
	return this->ebo.get_ebo();
}

GLuint MeshBuffer::get_vao_id() {
	return this->vao.get_id();
}

void MeshBuffer::erase() {
	this->vbo.Delete();
	this->ebo.Delete();
	this->vao.Delete();
}

bool MeshBuffer::empty() {
	return this->vbo.empty();
}

void MeshBuffer::append_vbo(GLfloat data) {
	this->vbo.append_data(data);
}

void MeshBuffer::append_ebo(GLuint data) {
	this->ebo.append_data(data);
}

void MeshBuffer::draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>& shader_table) {
	Shader shader = shader_table.at(layout.flags);

	std::cout << "drawing with layout.flags=" << layout.flags
		<< " shader id=" << shader.get_id() << std::endl;

	shader.Activate();

	cam.Matrix(fov, near, far, shader, "camMatrix");
	vao.bind();

	GLint enabled = 0;
	glGetVertexAttribiv(layout.loc_color, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
	void* ptr = nullptr;
	glGetVertexAttribPointerv(layout.loc_color, GL_VERTEX_ATTRIB_ARRAY_POINTER, &ptr);
	GLint stride = 0;
	glGetVertexAttribiv(layout.loc_color, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
	std::cout << "color attrib enabled=" << enabled
		<< " offset=" << ptr
		<< " stride=" << stride << std::endl;

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ebo.size()), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->get_ebo().size()));

	vao.unbind();
	shader.Deactivate();
}

BufferManager::~BufferManager() {
	for (auto it = meshes.begin(); it != meshes.end(); it++) {
		it->second.erase();
	}
}

std::unordered_map<uint32_t, MeshBuffer> BufferManager::get() {
	return this->meshes;
}

void BufferManager::clear() {
	this->meshes.clear();
}

void BufferManager::commit(std::unordered_map<unsigned short, mesh>& meshes) {
	int offset = 0;
	
	// Interweaving Buffer Data
	for (auto& it : meshes) {
		mesh& mesh = it.second;
		VertexLayout& layout = mesh.layout;
		MeshBuffer& mesh_buffer = this->meshes[layout.flags];

		mesh_buffer.layout = layout;

		int i, partition, n_strides, vector_index, stride_local_index;

		for (i = 0; i < mesh.vbo_size(); i++) {
			stride_local_index = i % layout.stride;
			partition = layout.getPositionInPartition(i % layout.stride);
			n_strides = i / layout.stride;

			if (partition == 0) {
				vector_index = (n_strides * 3) + stride_local_index;
				GLfloat data = mesh.cord_data[vector_index];
				mesh_buffer.append_vbo(data);
			}
			else if (partition == 1) {
				vector_index = (n_strides * layout.color_components()) + stride_local_index - layout.offset_color;
				GLfloat data = mesh.color_data[vector_index];
				mesh_buffer.append_vbo(data);
			}
			else if (partition == 2) {
				vector_index = (n_strides * 2) + stride_local_index - layout.offset_uv;
				GLfloat data = mesh.tex_data[vector_index];
				mesh_buffer.append_vbo(data);
			}
		}

		offset = (static_cast<int>(mesh_buffer.get_vbo().size()) / layout.stride) - (static_cast<int>(mesh.vbo_size()) / layout.stride);

		// Consider Object Offset of indices.
		// Each Mesh has it's local index but not
		// it's global buffer index.
		for (GLuint data : mesh.indices) {
			mesh_buffer.append_ebo(data + offset);
		}
	}

	// Sending to 
	for (auto& it : this->meshes) {
		MeshBuffer& mesh_buffer = it.second;
		VertexLayout& layout = it.second.layout;

		if (mesh_buffer.empty()) {
			continue;
		}

		mesh_buffer.vao.bind();

		mesh_buffer.vbo.bind();
		if (mesh_buffer.vbo.buffer_data()) {
			std::cout << "Failed to buffer vbo data" << std::endl;
			continue;
		}

		mesh_buffer.ebo.bind();
		if (mesh_buffer.ebo.buffer_data()) {
			std::cout << "Failed to buffer ebo data" << std::endl;
			continue;
		}

		for (GLenum err; (err = glGetError()) != GL_NO_ERROR; ) {
			std::cout << "Commit GL error after LinkAttribs: 0x" << std::hex << err << std::endl;
		}

		mesh_buffer.vao.LinkAttribs(layout);

		for (GLenum err; (err = glGetError()) != GL_NO_ERROR; ) {
			std::cout << "GL error after LinkAttribs: 0x" << std::hex << err << std::endl;
		}
		mesh_buffer.vao.unbind();
		mesh_buffer.ebo.unbind();
		mesh_buffer.vbo.unbind();
	}

	for (auto& mesh : this->meshes) {
		MeshBuffer& mesh_buffer = mesh.second;
		VertexLayout layout = mesh.second.layout;

		// ---- VBO readback ----
		mesh_buffer.vbo.bind();

		GLint vbo_size_bytes = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size_bytes);

		std::vector<GLfloat> gpu_vbo(vbo_size_bytes / sizeof(GLfloat));
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, vbo_size_bytes, gpu_vbo.data());

		std::cout << "vbo id: " << mesh_buffer.get_vbo_id()
			<< " (" << vbo_size_bytes << " bytes, "
			<< gpu_vbo.size() << " floats on GPU)" << std::endl;

		for (auto data : gpu_vbo) {
			std::cout << data << std::endl;
		}

		mesh_buffer.vbo.unbind();

		// ---- EBO readback ----
		mesh_buffer.ebo.bind();

		GLint ebo_size_bytes = 0;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &ebo_size_bytes);

		std::vector<GLuint> gpu_ebo(ebo_size_bytes / sizeof(GLuint));
		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ebo_size_bytes, gpu_ebo.data());

		std::cout << "ebo id: " << mesh_buffer.get_ebo_id()
			<< " (" << ebo_size_bytes << " bytes, "
			<< gpu_ebo.size() << " indices on GPU)" << std::endl;

		for (auto data : gpu_ebo) {
			std::cout << data << std::endl;
		}

		mesh_buffer.ebo.unbind();

		std::cout << "vao id: " << mesh_buffer.get_vao_id() << std::endl;
	}
}

void BufferManager::draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>& shader_table) {
	for (auto& it : this->meshes) {
		MeshBuffer& mesh_buffer = it.second;
		mesh_buffer.draw(cam, window, fov, near, far, shader_table);
	}
}