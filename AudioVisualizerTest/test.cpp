#include <vector>

#include "pch.h"

#include <glad/glad.h>

#include "../AudioVisualizer/objects.h"
#include "../AudioVisualizer/BufferBuilder.h"
#include "../AudioVisualizer/MultiCodecAudioDecoder.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(OBJECTS_TEST, ConstructorTest) {
	EXPECT_NO_THROW({ Objects obj; });
}

TEST(OBJECTS_TEST, ADD_OBJ_TEST) {
	Objects objs;

	std::vector<GLfloat> vbo = { 1.0f };
	std::vector<GLuint> ebo = { 1 };

	objs.add_obj(vbo, vbo, vbo, ebo);

	object obj = objs.get(0);

	EXPECT_EQ(obj.vertices, vbo);
	EXPECT_EQ(obj.rgb_data, vbo);
	EXPECT_EQ(obj.tex_data, vbo);
	EXPECT_EQ(obj.indices, ebo);
}

TEST(BUFFER_TEST, BUFFER_ONE_VERTEX) {
	Objects objs;

	std::vector<GLfloat> vbo = { 
								 -1.0f, 0.2f, 1.0f, 
								  0.5f, 0.7f, -0.5f,
								 -0.9f, 0.1f
	};

	std::vector<GLfloat> xyz = { - 1.0f, 0.2f, 1.0f };
	std::vector<GLfloat> rgb = { 0.5f, 0.7f, - 0.5f };
	std::vector<GLfloat> uv = { - 0.9f, 0.1f };
	std::vector<GLuint> ebo = { 1, 1, 0, 1, 0, 4, 6 };

	objs.add_obj(xyz, rgb, uv, ebo);

	buffer_objects(objs);


	EXPECT_EQ(objs.get_vbo_vector(), vbo);
	EXPECT_EQ(objs.get_ebo_vector(), ebo);
}