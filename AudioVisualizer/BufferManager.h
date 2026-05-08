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

	void erase();
	bool empty();

	void append_vbo(GLfloat data);
	void append_ebo(GLuint data);

	void buffer_data();

	void bind();
	void unbind();

	void begin(Camera& cam, float fov, float near, float far, std::unordered_map<uint32_t, Shader> shader_table);
	void draw();
};

class BufferManager {
private:
	std::unordered_map<uint32_t, MeshBuffers> meshes;
public:
	BufferManager() = default;
	~BufferManager();

	std::unordered_map<uint32_t, MeshBuffers> get();
	void clear();

	void commit(std::unordered_map<unsigned short, object>& objects);
	void draw(Camera& cam, float fov, float near, float far, std::unordered_map<uint32_t, Shader> shader_table);
};

#endif