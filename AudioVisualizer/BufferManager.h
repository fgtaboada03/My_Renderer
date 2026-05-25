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

	MeshBuffers() = default;
	~MeshBuffers() = default;

	bool empty();

	GLuint get_vbo_id();
	std::vector<GLfloat> get_vbo();
	GLuint get_ebo_id();
	std::vector<GLuint> get_ebo();
	GLuint get_vao_id();

	void erase();

	void append_vbo(GLfloat data);
	void append_ebo(GLuint data);

	void buffer_data();

	void bind();
	void unbind();

	void EnableAttribs();
	void DisableAttribs();

	void draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>& shader_table);
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
	void draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>& shader_table);
};

#endif