#include "BufferManager.h"

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

void BufferManager::upload(std::unordered_map<unsigned short, object>& objects) {
	for (auto it = objects.begin(); it != objects.end(); it++) {
		object obj = it->second;
		VertexLayout layout = obj.layout;
		MeshBuffers mesh_buffer = meshes[layout.flags];

		int i, partition, n_strides, vector_index, stride_local_index;

		std::cout << "obj.vbo_size() : " << obj.vbo_size() << std::endl;

		for (i = 0; i < obj.vbo_size(); i++) {
			stride_local_index = i % layout.stride;
			partition = layout.getPositionInPartition(i % layout.stride);
			n_strides = i / layout.stride;

			if (partition == 0) {
				vector_index = (n_strides * 3) + stride_local_index;
				GLfloat data = obj.cords_data[vector_index];

				std::cout << i << " : " << data << std::endl;

				mesh_buffer.vbo.append_data(data);
			}
			else if (partition == 1) {
				vector_index = (n_strides * layout.color_components()) + stride_local_index - layout.offset_color;
				GLfloat data = obj.color_data[vector_index];

				std::cout << i << " : " << data << std::endl;

				mesh_buffer.vbo.append_data(data);
			}
			else if (partition == 2) {
				vector_index = (n_strides * 2) + stride_local_index - layout.offset_uv;
				GLfloat data = obj.tex_data[vector_index];

				std::cout << i << " : " << data << std::endl;

				mesh_buffer.vbo.append_data(data);
			}
		}
		
		std::cout << "vbo filled" << std::endl;


		for (GLuint data : obj.indices) {
			mesh_buffer.ebo.append_data(data);
		}
	}



	for (auto it = meshes.begin(); it != meshes.end(); it++) {
		VertexLayout layout = it->second.layout;
		MeshBuffers mesh_buffer = it->second;

		std::cout << mesh_buffer.empty() << std::endl;

		if (mesh_buffer.empty()) {
			std::cout << "empty layout : " << layout.flags << std::endl;
			for (auto i : mesh_buffer.vbo.get_vbo()) {
				std::cout << i << std::endl;
			}
			continue;
		}

		std::cout << "full layout : " << layout.flags << std::endl;

		mesh_buffer.buffer_data();

		mesh_buffer.bind();

		// Link XYZ
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
	for (auto it = meshes.begin(); it != meshes.end(); it++) {
		MeshBuffers mesh_buffer = it->second;

		mesh_buffer.begin(cam, fov, near, far, shader_table);
		mesh_buffer.draw();
	}
}