#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <glad/glad.h>
#include <unordered_map>
#include <iostream>

#include "VertexLayout.h"
#include "shader.h"
#include "camera.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

// TODO determine if this even runs

struct MeshBuffers {
	VBO vbo;
	EBO ebo;
	VAO vao;
	VertexLayout layout;

	bool empty() {
		return vbo.empty();
	}

	void bind() {
		vbo.Bind();
		ebo.Bind();
		vao.Bind();
	}
	void unbind() {
		vbo.Unbind();
		ebo.Unbind();
		vao.Unbind();
	}

	void erase() {
		vbo.Delete();
		ebo.Delete();
		vao.Delete();
	}

	void buffer_data() {
		vbo.buffer_data();
		ebo.buffer_data();
	}

	void begin(Camera& cam, float fov, float near, float far, std::unordered_map<uint32_t, Shader> shader_table) {
		Shader shader = shader_table.at(layout.flags);
		shader.Activate();
		cam.Matrix(fov, near, far, shader, "camMatrix");
	}

	void draw() {
		bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ebo.size()), GL_UNSIGNED_INT, 0);
		unbind();
	}
};

class BufferManager {
private:
	std::unordered_map<uint32_t, MeshBuffers> meshes;
public:
	BufferManager() = default;
	~BufferManager();

	std::unordered_map<uint32_t, MeshBuffers> get();
	void clear();

	void upload(std::unordered_map<unsigned short, object>& objects);
	void draw(Camera& cam, float fov, float near, float far, std::unordered_map<uint32_t, Shader> shader_table);
};

#endif