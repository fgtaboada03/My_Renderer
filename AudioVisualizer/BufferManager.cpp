#include "BufferManager.h"

void MeshBuffers::erase() {
	vbo.Delete();
	ebo.Delete();
	vao.Delete();
}

bool MeshBuffers::empty() {
	return vbo.empty();
}

void MeshBuffers::append_vbo(GLfloat data) {
	this->vbo.append_data(data);
}

void MeshBuffers::append_ebo(GLuint data) {
	this->ebo.append_data(data);
}

void MeshBuffers::buffer_data() {
	vbo.buffer_data();
	ebo.buffer_data();
}

void MeshBuffers::bind() {
	vbo.Bind();
	ebo.Bind();
	vao.Bind();
}
void MeshBuffers::unbind() {
	vbo.Unbind();
	ebo.Unbind();
	vao.Unbind();
}

void MeshBuffers::begin(Camera& cam, float fov, float near, float far, std::unordered_map<uint32_t, Shader> shader_table) {
	Shader shader = shader_table.at(layout.flags);
	shader.Activate();
	cam.Matrix(fov, near, far, shader, "camMatrix");
}

void MeshBuffers::draw() {
	bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ebo.size()), GL_UNSIGNED_INT, 0);
	unbind();
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
	meshes.clear();
}

void BufferManager::commit(std::unordered_map<unsigned short, object>& objects) {
	VertexLayout layout;

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

		for (GLuint data : obj.indices) {
			mesh_buffer.append_ebo(data);
		}
	}

	for (auto mesh : meshes) {
		MeshBuffers& mesh_buffer = mesh.second;
		layout = mesh.second.layout;

		if (mesh_buffer.empty()) {
			continue;
		}

		mesh_buffer.buffer_data();
		mesh_buffer.bind();

		// Link XYZ
		VertexLayout& layout = mesh.second.layout;
		mesh_buffer.vao.LinkAttrib(mesh_buffer.vbo, layout.loc_xyz, 3, GL_FLOAT, layout.stride, (void*)0);

		// Link Color
		if (layout.has_color()) {
			mesh_buffer.vao.LinkAttrib(mesh_buffer.vbo, layout.loc_color, layout.color_components(), GL_FLOAT, layout.stride, (void*)(layout.offset_color * sizeof(float)));
		}

		// Link UV
		if (layout.has_uv()) {
			mesh_buffer.vao.LinkAttrib(mesh_buffer.vbo, layout.loc_uv, 2, GL_FLOAT, layout.stride, (void*)(layout.offset_uv * sizeof(float)));
		}

		mesh_buffer.unbind();
	}
}

void BufferManager::draw(Camera& cam, float fov, float near, float far, std::unordered_map<uint32_t, Shader> shader_table) {
	for (auto& it : meshes) {
		MeshBuffers mesh_buffer = it.second;

		mesh_buffer.begin(cam, fov, near, far, shader_table);
		mesh_buffer.draw();
	}
}