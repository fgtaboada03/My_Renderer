#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <glad/glad.h>
#include <unordered_map>
#include <queue>

#include "VertexLayout.h"
#include "texture.h"
#include "shader.h"


class MeshManager {
private:
	std::unordered_map<unsigned short, mesh> meshes;
	std::queue<unsigned short> free_pool;
	unsigned short count{ 0 };

public:
	MeshManager() = default;
	~MeshManager() = default;

	mesh& at(unsigned short id) { return this->meshes.at(id); }
	const mesh& at(unsigned short id) const { return this->meshes.at(id); }

	std::unordered_map<unsigned short, mesh>& get_meshes() { return this->meshes; }
	const std::unordered_map<unsigned short, mesh>& get_meshes() const { return this->meshes; }

	void clear();
	void delete_mesh(unsigned short id);

	void append_cord_data(unsigned short id, std::vector<GLfloat> cords_data);
	void append_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba = false);
	void append_tex_data(unsigned short id, std::vector<GLfloat> tex_data);
	void append_indices(unsigned short id, std::vector<GLuint> indices);

	void replace_cord_data(unsigned short id, std::vector<GLfloat> cords_data);
	void replace_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba = false);
	void replace_tex_data(unsigned short id, std::vector<GLfloat> tex_data);
	void replace_indices(unsigned short id, std::vector<GLuint> indices);

	unsigned short new_mesh();

	void add_mesh(mesh& mesh);
	void add_mesh(std::vector<GLfloat> cords, std::vector<GLfloat> color, std::vector<GLfloat> tex, std::vector<GLuint> indices, bool is_rgba = false);
};

#endif
