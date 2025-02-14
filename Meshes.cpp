#include "Meshes.hpp"

void Mesh::addFaceByIndices(GLuint i1, GLuint i2, GLuint i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

Mesh uvSphereMesh(int subdivisions) {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	float stepsize = 360.0 / subdivisions;

	// top pole
	vertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

	// add points between the poles
	for (int j = 1; j < subdivisions; j++) {
		float phi = glm::radians(j * stepsize / 2 - 90);
		float z = sin(phi);
		for (int i = 0; i < subdivisions; i++) {
			float theta = glm::radians(i * stepsize);
			float x = cos(phi) * cos(theta);
			float y = cos(phi) * sin(theta);
			vertices.push_back(glm::vec3(x, y, z));
		}
	}

	// bottom pole 
	vertices.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

	// indices top pole
	for (int i = 1; i <= subdivisions; i++) {
		indices.push_back(0); // top pole
		indices.push_back(i); // first on next ring
		indices.push_back(i % subdivisions + 1); // second on next ring
	}

	// indices bottom pole
	int bottomPoleIndex = vertices.size() - 1;
	for (int i = 1; i <= subdivisions; i++) {
		indices.push_back(bottomPoleIndex); // bottom pole
		indices.push_back(bottomPoleIndex - i); // first on previous ring
		indices.push_back(bottomPoleIndex - (i % subdivisions + 1)); // second on previous ring
	}

	// indices middle
	for (int j = 0; j < subdivisions - 2; j++) {
		for (int i = 0; i < subdivisions; i++) {
			int currentRing = j * subdivisions + 1;
			int nextRing = (j + 1) * subdivisions + 1;
			int next = (i + 1) % subdivisions;

			indices.push_back(currentRing + i);
			indices.push_back(nextRing + i);
			indices.push_back(currentRing + next);

			indices.push_back(currentRing + next);
			indices.push_back(nextRing + i);
			indices.push_back(nextRing + next);
		}
	}
	Mesh mesh;
	mesh.vertices = vertices;
	mesh.indices = indices;
	for (int i = 0; i < vertices.size(); i++) {
		mesh.normals.push_back(glm::normalize(vertices[i]));
	}

	return mesh;
}

Mesh initialiIcoSphereMesh() {
	// vertices of an Icosahedron 
	// https://en.wikipedia.org/wiki/Icosahedron
	std::vector<glm::vec3> sphereVertices;

	sphereVertices.push_back(glm::vec3(-1, GOLDEN_RATIO, 0));
	sphereVertices.push_back(glm::vec3(1, GOLDEN_RATIO, 0));
	sphereVertices.push_back(glm::vec3(-1, -GOLDEN_RATIO, 0));
	sphereVertices.push_back(glm::vec3(1, -GOLDEN_RATIO, 0));

	sphereVertices.push_back(glm::vec3(0, -1, GOLDEN_RATIO));
	sphereVertices.push_back(glm::vec3(0, 1, GOLDEN_RATIO));
	sphereVertices.push_back(glm::vec3(0, -1, -GOLDEN_RATIO));
	sphereVertices.push_back(glm::vec3(0, 1, -GOLDEN_RATIO));

	sphereVertices.push_back(glm::vec3(GOLDEN_RATIO, 0, -1));
	sphereVertices.push_back(glm::vec3(GOLDEN_RATIO, 0, 1));
	sphereVertices.push_back(glm::vec3(-GOLDEN_RATIO, 0, -1));
	sphereVertices.push_back(glm::vec3(-GOLDEN_RATIO, 0, 1));

	Mesh mesh;
	mesh.vertices = sphereVertices;

	std::vector<GLuint> sphereIndices;
	mesh.addFaceByIndices(0, 11, 5);
	mesh.addFaceByIndices(0, 5, 1);
	mesh.addFaceByIndices(0, 1, 7);
	mesh.addFaceByIndices(0, 7, 10);
	mesh.addFaceByIndices(0, 10, 11);

	mesh.addFaceByIndices(1, 5, 9);
	mesh.addFaceByIndices(5, 11, 4);
	mesh.addFaceByIndices(11, 10, 2);
	mesh.addFaceByIndices(10, 7, 6);
	mesh.addFaceByIndices(7, 1, 8);

	mesh.addFaceByIndices(3, 9, 4);
	mesh.addFaceByIndices(3, 4, 2);
	mesh.addFaceByIndices(3, 2, 6);
	mesh.addFaceByIndices(3, 6, 8);
	mesh.addFaceByIndices(3, 8, 9);

	mesh.addFaceByIndices(4, 9, 5);
	mesh.addFaceByIndices(2, 4, 11);
	mesh.addFaceByIndices(6, 2, 10);
	mesh.addFaceByIndices(8, 6, 7);
	mesh.addFaceByIndices(9, 8, 1);

	return mesh;
}
