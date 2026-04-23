#ifndef OBJECTS_H
#define OBJECTS_H

#include <glad/glad.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>

#include <iostream>

struct object {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> rgb_data;
	std::vector<GLfloat> tex_data;
	std::vector<GLuint>  indices;
	bool rgba = false;
};

class Objects {
private:
	std::unordered_map<unsigned short, object> objects;
	int count = 0;
	std::queue<unsigned short> free_pool;
	std::vector<GLfloat> vbo;
	std::vector<GLuint> ebo;
	int vbo_size = 0;
	int ebo_size = 0;

public:
	Objects() = default;
	~Objects() = default;
	
	void clear();
	void clear_objects();
	void clear_buffers();

	struct object get(unsigned short id);
	std::unordered_map<unsigned short, object> get_objects();
	std::vector<GLfloat> get_vbo_vector();
	std::vector<GLuint> get_ebo_vector();
	GLfloat* get_vbo_array();
	GLuint* get_ebo_array();
	int get_vbo_size();
	int get_ebo_size();
	int get_byte_size_vertices();
	int get_byte_size_indices();
	
	void push_vbo(GLfloat point);
	void push_ebo(GLuint index);

	void set_vertex_buffer_size(int size);
	void set_index_buffer_size(int size);

	void delete_object(unsigned short id);
	unsigned short get_new_id();

	void add_vertices(unsigned short id, std::vector<GLfloat> vertices);
	void add_rgb_data(unsigned short id, std::vector<GLfloat> rgb_data, bool rgba = false);
	void add_tex_data(unsigned short id, std::vector<GLfloat> tex_data);
	void add_indices(unsigned short id, std::vector<GLuint> indices);

	void add_obj(std::vector<GLfloat> vertices, std::vector<GLfloat> rgb_data, std::vector<GLfloat> tex_data, std::vector<GLuint> indices);
	void add_triangle(int scale = 1, float offsetX = 0, float offsetY = 0, float offsetZ = 0);
	void add_square(int scale = 1, float offsetX = 0, float offsetY = 0, float offsetZ = 0);
	void add_cube(int scale = 1, float offsetX = 0, float offsetY = 0, float offsetZ = 0);
};

#endif