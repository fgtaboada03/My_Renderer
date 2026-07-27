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

int main() {
	// set up vertex data (and buffer(s)) and configure vertex attributes

	int intial_scr_width = 800;
	int intial_scr_height = 800;

	Engine engine(intial_scr_width, intial_scr_height);

	std::cout << "MAIN 32" << std::endl;

	if (engine.init()) {
		return 1;
	}

	std::cout << "MAIN 34" << std::endl;

	VertexLayout layout = VertexLayout(VERTEX_XYZ | VERTEX_RGB);

	std::cout << "MAIN 36" << std::endl;

	mesh tri = triangle(layout);

	std::cout << "MAIN 38" << std::endl;

 	engine.add_mesh(tri);

	std::cout << "MAIN 40" << std::endl;

	engine.commit();

	// Texture Stuff
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	//Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	// render loop
	// -----------
	engine.run();

	//popCat.Delete();

	return 0;
}