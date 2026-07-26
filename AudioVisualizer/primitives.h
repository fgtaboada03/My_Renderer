#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "VertexLayout.h"

inline mesh triangle(VertexLayout layout = VertexLayout(VERTEX_XYZ), int scale = 1, float offsetX = 0, float offsetY = 0, float offsetZ = 0) {
	std::vector<GLfloat> xyz = {
		(0.5f * scale) + offsetX,  (-0.5f * scale) + offsetY,   (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX,  (-0.5f * scale) + offsetY,   (0.5f * scale) + offsetZ,
		(0.0f * scale) + offsetX,  (0.5f * scale) + offsetY,   (0.5f * scale) + offsetZ
	};

	std::vector<GLfloat> rgb = {
		1.0f, 0.5f, 0.2f,
		1.0f, 0.5f, 0.2f,
		1.0f, 0.5f, 0.2f
	};

	std::vector<GLfloat> uv = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	std::vector<GLuint> indices = {
		0, 1, 2
	};

	mesh mesh(layout, xyz, indices, rgb, uv);

	return mesh;
}

inline mesh square(VertexLayout layout = VertexLayout(VERTEX_XYZ), int scale = 1, float offsetX = 0, float offsetY = 0, float offsetZ = 0) {
	std::vector<GLfloat> xyz = {
		(0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(0.5f * scale) + offsetX, (0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ
	};

	std::vector<GLfloat> rgb = {
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	std::vector<GLfloat> uv = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	std::vector<GLuint> indices = {
		0, 1, 2,
		3, 2, 1
	};

	mesh mesh(layout, xyz, indices, rgb, uv);

	return mesh;
}

inline mesh cube(VertexLayout layout = VertexLayout(VERTEX_XYZ), int scale = 1, float offsetX = 0, float offsetY = 0, float offsetZ = 0) {
	std::vector<GLfloat> xyz = {
		(0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(0.5f * scale) + offsetX, (0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (0.5f * scale) + offsetY,  (0.5f * scale) + offsetZ,
		(0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ,
		(0.5f * scale) + offsetX, (0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (-0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ,
		(-0.5f * scale) + offsetX, (0.5f * scale) + offsetY,  (-0.5f * scale) + offsetZ
	};
	std::vector<GLfloat> rgb = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	};
	std::vector<GLfloat> uv = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};
	std::vector<GLuint> indices = {
		0, 1, 2,
		3, 2, 1,
		4, 5, 6,
		7, 5, 6,
		4, 5, 0,
		1, 0, 5,
		6, 7, 2,
		3, 2, 7,
		1, 3, 5,
		7, 5, 3,
		0, 2, 4,
		6, 4, 2
	};

	mesh mesh(layout, xyz, indices, rgb, uv);

	return mesh;
}

#endif