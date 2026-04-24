#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <glad/glad.h>
#include <unordered_map>
#include <queue>

#include "VertexLayout.h"
#include "texture.h"
#include "shader.h"


class ObjectManager {
private:
	std::unordered_map<unsigned short, object> objects;
	std::queue<unsigned short> free_pool;
	unsigned short count{ 0 };

public:
	object& at(unsigned short id) { return this->objects.at(id); }
	const object& at(unsigned short id) const { return this->objects.at(id); }

	std::unordered_map<unsigned short, object>& get_objects() { return this->objects; }
	const std::unordered_map<unsigned short, object>& get_objects() const { return this->objects; }

	void clear();
	void delete_object(unsigned short id);

	void append_cords_data(unsigned short id, std::vector<GLfloat> cords_data);
	void append_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba = false);
	void append_tex_data(unsigned short id, std::vector<GLfloat> tex_data);
	void append_indices(unsigned short id, std::vector<GLuint> indices);

	void replace_cords_data(unsigned short id, std::vector<GLfloat> cords_data);
	void replace_color_data(unsigned short id, std::vector<GLfloat> color_data, bool is_rgba = false);
	void replace_tex_data(unsigned short id, std::vector<GLfloat> tex_data);
	void replace_indices(unsigned short id, std::vector<GLuint> indices);

	unsigned short new_object();

	void add_obj(object& obj);
	void add_obj(std::vector<GLfloat> cords, std::vector<GLfloat> color, std::vector<GLfloat> tex, std::vector<GLuint> indices, bool is_rgba = false);
};

#endif
