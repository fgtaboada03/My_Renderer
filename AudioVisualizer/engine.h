#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <unordered_map>
#include <vector>

#include "BufferManager.h"
#include "ObjectManager.h"
#include "shader.h"

// ------------------------------------------------------------
//  Objects — Controls Object and Buffer Manager
// ------------------------------------------------------------
class Engine {
private:
	ObjectManager object_manager;
	BufferManager buffer_manager;
	const std::unordered_map<uint32_t, Shader> SHADER_TABLE = {
		{ VERTEX_XYZ							, Shader("default.vert", "default.frag") },
		{ VERTEX_XYZ | VERTEX_RGB				, Shader("default.vert", "default.frag") },
		{ VERTEX_XYZ | VERTEX_RGB | VERTEX_UV	, Shader("default.vert", "default.frag") },
		{ VERTEX_XYZ | VERTEX_RGBA				, Shader("default.vert", "default.frag") },
		{ VERTEX_XYZ | VERTEX_RGBA | VERTEX_UV	, Shader("default.vert", "default.frag") },
		{ VERTEX_XYZ | VERTEX_UV				, Shader("default.vert", "default.frag") }
	};

public:
	Engine() = default;
	~Engine() = default;
	
	ObjectManager get_object_mng() { return this->object_manager; }
	BufferManager get_buffer_mng() { return this->buffer_manager; }

	void clear() {
		this->object_manager.clear();
		this->buffer_manager.clear();
	}
	struct object get(unsigned short id) { return this->object_manager.at(id); }
	std::unordered_map<unsigned short, object>& get_objects() { return this->object_manager.get_objects(); }
	void delete_object(unsigned short id) { this->object_manager.delete_object(id); };

	void append_cords_data(unsigned short id, std::vector<GLfloat> cords_data) { this->object_manager.append_cords_data(id, cords_data); }
	void append_color_data(unsigned short id, std::vector<GLfloat> color_data, bool rgba = false) { this->object_manager.append_color_data(id, color_data, rgba); }
	void append_tex_data(unsigned short id, std::vector<GLfloat> tex_data) { this->object_manager.append_tex_data(id, tex_data); }
	void append_indices(unsigned short id, std::vector<GLuint> indices) { this->object_manager.append_indices(id, indices); }
	
	void replace_cords_data(unsigned short id, std::vector<GLfloat> cords_data) { this->object_manager.replace_cords_data(id, cords_data); }
	void replace_color_data(unsigned short id, std::vector<GLfloat> color_data, bool rgba = false) { this->object_manager.replace_color_data(id, color_data, rgba); }
	void replace_tex_data(unsigned short id, std::vector<GLfloat> tex_data) { this->object_manager.replace_tex_data(id, tex_data); }
	void replace_indices(unsigned short id, std::vector<GLuint> indices) { this->object_manager.replace_indices(id, indices); }

	void add_obj(object obj) { object_manager.add_obj(obj); }
	void add_obj(std::vector<GLfloat> cords, std::vector<GLfloat> color, std::vector<GLfloat> tex, std::vector<GLuint> indices, bool is_rgba = false) {
		this->object_manager.add_obj(cords, color, tex, indices, is_rgba);
	}

	void commit() { this->buffer_manager.upload(this->object_manager.get_objects()); }
	void draw(Camera& cam, float fov, float near, float far) { this->buffer_manager.draw(cam, fov, near, far, SHADER_TABLE); }
};

#endif