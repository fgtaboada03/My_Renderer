#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <vector>

#include "BufferManager.h"
#include "MeshManager.h"
#include "shader.h"

// ------------------------------------------------------------
//  Objects — Controls Object and Buffer Manager
// ------------------------------------------------------------
class Engine {
private:
	int SCR_WIDTH;
	int SCR_HEIGHT;
	std::unordered_map<uint32_t, Shader> SHADER_TABLE;

	GLFWwindow* window = nullptr;
	Camera camera;

	MeshManager mesh_manager;
	BufferManager buffer_manager;

public:
	Engine(int screen_width, int screen_height) :
		SCR_WIDTH(screen_width),
		SCR_HEIGHT(screen_height),
		camera (SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f))
	{}
	~Engine() {
		glfwTerminate();
	}

	int init() {
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GoobyGoop", NULL, NULL);
		// Error Check
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return 1;	
		}
		// Introduce Window to current Context
		glfwMakeContextCurrent(window);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return 1;
		};
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		SHADER_TABLE.clear();

		SHADER_TABLE.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(VERTEX_XYZ),
			std::forward_as_tuple("cords.vert", "cords.frag"));

		SHADER_TABLE.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(VERTEX_XYZ | VERTEX_RGB),
			std::forward_as_tuple("rgb.vert", "rgb.frag"));

		SHADER_TABLE.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(VERTEX_XYZ | VERTEX_RGB | VERTEX_UV),
			std::forward_as_tuple("rgb_uv.vert", "rgb_uv.frag"));

		SHADER_TABLE.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(VERTEX_XYZ | VERTEX_RGB),
			std::forward_as_tuple("rgba.vert", "rgba.frag"));

		SHADER_TABLE.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(VERTEX_XYZ | VERTEX_RGBA | VERTEX_UV),
			std::forward_as_tuple("rgba_uv.vert", "rgba_uv.frag"));

		// TODO : Won't construct uv shader. Weird little bug.

		SHADER_TABLE.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(VERTEX_XYZ | VERTEX_UV),
			std::forward_as_tuple("uv.vert", "uv.frag"));

		return 0;
	}
	
	void run(float fov = 45.0f, float near = 0.1f, float far = 100.0f) {
		// Depth Test
		glEnable(GL_DEPTH_TEST);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window)) {
			// render
			// -----------
			this->camera.Inputs(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//popCat.Bind();

			this->buffer_manager.draw(this->camera, this->window, fov, near, far, &SHADER_TABLE);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void commit() { this->buffer_manager.commit(this->mesh_manager.get_meshes()); }

	MeshManager get_mesh_mng() { return this->mesh_manager; }
	BufferManager get_buffer_mng() { return this->buffer_manager; }

	void clear() {
		this->mesh_manager.clear();
		this->buffer_manager.clear();
	}
	struct mesh get(unsigned short id) { return this->mesh_manager.at(id); }
	std::unordered_map<unsigned short, mesh>& get_meshes() { return this->mesh_manager.get_meshes(); }
	void delete_mesh(unsigned short id) { this->mesh_manager.delete_mesh(id); };

	void append_cord_data(unsigned short id, std::vector<GLfloat> cords_data) { this->mesh_manager.append_cord_data(id, cords_data); }
	void append_color_data(unsigned short id, std::vector<GLfloat> color_data, bool rgba = false) { this->mesh_manager.append_color_data(id, color_data, rgba); }
	void append_tex_data(unsigned short id, std::vector<GLfloat> tex_data) { this->mesh_manager.append_tex_data(id, tex_data); }
	void append_indices(unsigned short id, std::vector<GLuint> indices) { this->mesh_manager.append_indices(id, indices); }
	
	void replace_cord_data(unsigned short id, std::vector<GLfloat> cords_data) { this->mesh_manager.replace_cord_data(id, cords_data); }
	void replace_color_data(unsigned short id, std::vector<GLfloat> color_data, bool rgba = false) { this->mesh_manager.replace_color_data(id, color_data, rgba); }
	void replace_tex_data(unsigned short id, std::vector<GLfloat> tex_data) { this->mesh_manager.replace_tex_data(id, tex_data); }
	void replace_indices(unsigned short id, std::vector<GLuint> indices) { this->mesh_manager.replace_indices(id, indices); }

	void add_mesh(mesh mesh) { mesh_manager.add_mesh(mesh); }
	void add_mesh(std::vector<GLfloat> cords, std::vector<GLfloat> color, std::vector<GLfloat> tex, std::vector<GLuint> indices, bool is_rgba = false) {
		this->mesh_manager.add_mesh(cords, color, tex, indices, is_rgba);
	}
};

#endif