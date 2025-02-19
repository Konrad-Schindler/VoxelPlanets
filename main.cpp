#include "Window.hpp"
#include <glm/vec3.hpp>
#include <vector>
#include "Shader.hpp"
#include "PlanetSystem.hpp"
#include "Meshes.hpp"


int trajectorySteps = 10000;
std::vector<glm::vec3> colors =
{
	glm::vec3(1, 1, 0.3),
	glm::vec3(0.f, 1.f, 0.f),
	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(1.f, 1.f, 0.f),
	glm::vec3(1.f, 0.f, 1.f),
	glm::vec3(0.f, 1.f, 1.f),
	glm::vec3(0.5f, 0.f, 0.f),
	glm::vec3(0.f, 0.5f, 0.f),
	glm::vec3(0.f, 0.f, 0.5f),
	glm::vec3(0.5f, 0.5f, 0.f),
	glm::vec3(0.5f, 0.f, 0.5f),
	glm::vec3(0.f, 0.5f, 0.5f),
	glm::vec3(0.5f, 0.5f, 0.5f),
};


std::vector<uint8_t> voxelVolumeSphere(int radius) {
	int diameter = 2 * radius;
	std::vector<uint8_t> volume(diameter * diameter * diameter, 0);
	for (int i = 0; i < diameter; i++) {
		for (int j = 0; j < diameter; j++) {
			for (int k = 0; k < diameter; k++) {
				int x = i - radius;
				int y = j - radius;
				int z = k - radius;
				if (x * x + y * y + z * z <= radius * radius) {
					volume[i * diameter * diameter + j * diameter + k] = 255;
				}
			}
		}
	}
	return volume;
}

Mesh boundingBoxSphere() {
	Mesh mesh;
	std::vector<glm::vec3>* vertices = &mesh.vertices;
	vertices->push_back(glm::vec3(0, 0, 0));
	vertices->push_back(glm::vec3(19, 0, 0));
	vertices->push_back(glm::vec3(0, 19, 0));
	vertices->push_back(glm::vec3(19, 19, 0));
	vertices->push_back(glm::vec3(0, 0, 19));
	vertices->push_back(glm::vec3(19, 0, 19));
	vertices->push_back(glm::vec3(0, 19, 19));
	vertices->push_back(glm::vec3(19, 19, 19));

	mesh.addFaceByIndices(0, 1, 2);
	mesh.addFaceByIndices(1, 3, 2);
	mesh.addFaceByIndices(4, 6, 5);
	mesh.addFaceByIndices(5, 6, 7);
	mesh.addFaceByIndices(0, 4, 1);
	mesh.addFaceByIndices(1, 4, 5);
	mesh.addFaceByIndices(2, 3, 6);
	mesh.addFaceByIndices(3, 7, 6);
	mesh.addFaceByIndices(0, 2, 4);
	mesh.addFaceByIndices(2, 6, 4);
	mesh.addFaceByIndices(1, 5, 3);
	mesh.addFaceByIndices(3, 5, 7);

	return mesh;
}


int main()
{
	Window window;
	if (!window.initialized)
		return -1;

	window.camera.position = glm::vec3(-50, 20, -30);
	window.camera.lookAt(glm::vec3(0.f, 0.f, 0.f));

	PlanetSystem planetSystem;
	planetSystem.addPlanet(
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.f),
		10.f,
		2.f
	);
	/*planetSystem.addPlanet(
		glm::vec3(4.f, 0.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.5f),
		2.f,
		1.f
	);*/
	/*planetSystem.addPlanet(
		glm::vec3(4.f, 0.f, -10.f),
		glm::vec3(0.0f, 0.f, 0.3f),
		1.f,
		1.f
	);
	planetSystem.addPlanet(
		glm::vec3(-4.f, 0.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.5f),
		1.f,
		2.f
	);
	planetSystem.addPlanet(
		glm::vec3(1.f, -5.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.5f),
		2.f,
		2.f
	);*/

	Shader shader("C:/Users/Konrad/source/repos/Ants/Shader Files/shader.vert", "C:/Users/Konrad/source/repos/Ants/Shader Files/shader.frag");
	GLuint VAO, buffer, indexbuffer, trajectoryVAO, trajectoryBuffer, normalBuffer, pointsVAO, pointsBuffer;
	GLuint VAOs[2];
	glCreateVertexArrays(2, VAOs);
	VAO = VAOs[0];
	trajectoryVAO = VAOs[1];

	Mesh boundingBox = boundingBoxSphere();
	GLuint buffers[4];
	glCreateBuffers(4, buffers);
	buffer = buffers[0];
	normalBuffer = buffers[1];
	indexbuffer = buffers[2];
	trajectoryBuffer = buffers[3];
	glNamedBufferStorage(buffer, sizeof(glm::vec3) * boundingBox.vertices.size(), boundingBox.vertices.data(), GL_MAP_READ_BIT);
	glNamedBufferStorage(normalBuffer, sizeof(glm::vec3) * boundingBox.vertices.size(), boundingBox.normals.data(), GL_MAP_READ_BIT);
	glNamedBufferStorage(indexbuffer, sizeof(GLuint) * boundingBox.indices.size(), boundingBox.indices.data(), GL_MAP_READ_BIT);
	glNamedBufferStorage(trajectoryBuffer, sizeof(glm::vec3) * trajectorySteps, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glVertexArrayVertexBuffer(VAO, 0, buffer, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayVertexBuffer(VAO, 1, normalBuffer, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(VAO, 1);

	glBindVertexArray(trajectoryVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trajectoryBuffer);
	glVertexArrayVertexBuffer(trajectoryVAO, 0, trajectoryBuffer, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(trajectoryVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(trajectoryVAO, 0);


	GLuint voxelVolume;
	glGenTextures(1, &voxelVolume);
	glBindTexture(GL_TEXTURE_3D, voxelVolume);
	auto volume = voxelVolumeSphere(10);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, 20, 20, 20, 0, GL_RED, GL_UNSIGNED_BYTE, volume.data());
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glm::mat4 projection = glm::perspective(glm::radians(60.0), 16.0/9.0, 0.1, 1000.0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// sglEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPointSize(20.0f);
	while (!window.shouldClose())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		shader.uniformMatrix4("projection", projection);
		shader.uniformMatrix4("view", window.camera.getView());
		shader.uniformVector3("viewPos", window.camera.position);
		shader.uniformInt("windowHeight", window.height);
		shader.uniformInt("windowWidth", window.width);

		auto trajectories = planetSystem.calculateTrajectories(trajectorySteps, 2);

		planetSystem.update(window.deltaTime);
		for (int i = 0; i < planetSystem.planets.size(); i++) {
			shader.uniformVector3("planetColor", colors[i % planetSystem.planets.size()]);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, planetSystem.planets[i].position);

			shader.uniformMatrix4("model", model);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, boundingBox.indices.size(), GL_UNSIGNED_INT, 0);
			if (window.debugInformation.showTrajectories) {
				glNamedBufferSubData(trajectoryBuffer, 0, sizeof(glm::vec3) * trajectorySteps, trajectories[i].data());
				glBindVertexArray(trajectoryVAO);
				shader.uniformMatrix4("model", glm::mat4(1));
				glDrawArrays(GL_LINE_STRIP, 0, trajectories[i].size());
			}
		}

		window.runLoopFunctions();
	}
}

