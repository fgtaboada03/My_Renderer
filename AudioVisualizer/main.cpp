#include<filesystem>
namespace fs = std::filesystem;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"
#include "Texture.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "MultiCodecAudioDecoder.h"
#include "objects.h"
#include "BufferBuilder.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GoobyGoop", NULL, NULL);
	// Error Check
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce Window to current Context
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	};
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	Shader shaderProgram_1("default.vert", "default.frag");
	Shader shaderProgram_2("default.vert", "yellow.frag");
	
	VAO VAO1;
	VAO1.Bind();

	Objects objs;
	objs.add_square();
	if (buffer_objects(objs)) {
		std::cout << "Failed to buffer objects" << std::endl;
	}
	GLfloat* vertices = objs.get_vbo_array();
	GLuint* indices = objs.get_ebo_array();

	std::cout << "Object Square" << std::endl;


	for (GLfloat vertex : objs.get_vbo_vector()) {
		std::cout << "vertex " << ": " << vertex << std::endl;
	}

	for (GLuint index : objs.get_ebo_vector()) {
		std::cout << "index " << ": " << index << std::endl;
	}

	for (int i = 0; i < objs.get_vbo_size(); i++) {
		std::cout << "vertex " << i << ": " << vertices[i] << std::endl;
	}

	for (int i = 0; i < objs.get_ebo_size(); i++) {
		std::cout << "index " << i << ": " << indices[i] << std::endl;
	}

	VBO VBO1(vertices, objs.get_byte_size_vertices());
	EBO EBO1(indices, objs.get_byte_size_indices());

	objs.clear_objects();

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbinds all to prevent accidental modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Texture Stuff
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	//Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	//popCat.texUnit(shaderProgram_1, "tex0", 0);

	// Depth Test

	glEnable(GL_DEPTH_TEST);

	Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// render
		// -----------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram_1.Activate();

		camera.Inputs(window);

		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram_1, "camMatrix"); 

		//popCat.Bind();

		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(objs.get_ebo_size()), GL_UNSIGNED_INT, 0);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//popCat.Delete();
	shaderProgram_1.Delete();
	shaderProgram_2.Delete();

	glfwTerminate();
	return 0;
}