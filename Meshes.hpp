#pragma once

#include <glad/glad.h>
#include <utility>
#include <vector>
#include <glm/glm.hpp>

const float GOLDEN_RATIO = (1 + sqrt(5)) / 2;

struct Mesh {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;
	void addFaceByIndices(GLuint i1, GLuint i2, GLuint i3);
};

Mesh uvSphereMesh(int subdivisions);
// Mesh icoSphereMesh(int subdivisions);
