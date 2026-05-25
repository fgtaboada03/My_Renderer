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
#include "MultiCodecAudioDecoder.h"
#include "engine.h"
#include "primitives.h"
#include "VertexLayout.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
	// set up vertex data (and buffer(s)) and configure vertex attributes
	Engine engine(SCR_WIDTH, SCR_HEIGHT);

	if (engine.init()) {
		return 1;
	}

	VertexLayout layout = VertexLayout(VERTEX_XYZ | VERTEX_RGB);

	object shape = triangle(layout);

	engine.add_obj(shape);
	std::cout << "object stage" << std::endl;
	engine.commit();
	std::cout << "object committed" << std::endl;

	// Texture Stuff
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	//Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	// render loop
	// -----------
	std::cout << "running" << std::endl;
	engine.run();
	std::cout << "program end" << std::endl;	

	//popCat.Delete();

	return 0;
}