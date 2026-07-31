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

struct MeshBuffer {
	VBO vbo;
	EBO ebo;
	VAO vao;
	VertexLayout layout;

	MeshBuffer() = default;
	~MeshBuffer() = default;


	bool empty();

	std::vector<GLfloat> get_vbo();
	std::vector<GLuint> get_ebo();

	GLuint get_vbo_id();
	GLuint get_ebo_id();
	GLuint get_vao_id();

	void erase();

	void append_vbo(GLfloat data);
	void append_ebo(GLuint data);

	void draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>* shader_table);
};

class BufferManager {
private:
	std::unordered_map<uint32_t, MeshBuffer> meshes;
public:
	BufferManager() = default;
	~BufferManager();

	std::unordered_map<uint32_t, MeshBuffer> get();
	void clear();

	void commit(std::unordered_map<unsigned short, mesh>& meshes);
	void draw(Camera& cam, GLFWwindow* window, float fov, float near, float far, std::unordered_map<uint32_t, Shader>* shader_table);
};

#endif