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

int main()
{
	Window window;
	if (!window.initialized)
		return -1;

	window.camera.position = glm::vec3(5.f, 3.f, 5.f);
	window.camera.lookAt(glm::vec3(0.f, 0.f, 0.f));

	PlanetSystem planetSystem;
	planetSystem.addPlanet(
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.f),
		10.f,
		2.f
	);
	planetSystem.addPlanet(
		glm::vec3(4.f, 0.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.5f),
		2.f,
		1.f
	);
	planetSystem.addPlanet(
		glm::vec3(4.f, 0.f, -10.f),
		glm::vec3(0.0f, 0.f, 0.3f),
		1.f,
		1.f
	);
	/*planetSystem.addPlanet(
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

	Mesh sphere = uvSphereMesh(20);
	GLuint buffers[4];
	glCreateBuffers(4, buffers);
	buffer = buffers[0];
	normalBuffer = buffers[1];
	indexbuffer = buffers[2];
	trajectoryBuffer = buffers[3];
	glNamedBufferStorage(buffer, sizeof(glm::vec3) * sphere.vertices.size(), sphere.vertices.data(), GL_MAP_READ_BIT);
	glNamedBufferStorage(normalBuffer, sizeof(glm::vec3) * sphere.vertices.size(), sphere.normals.data(), GL_MAP_READ_BIT);
	glNamedBufferStorage(indexbuffer, sizeof(GLuint) * sphere.indices.size(), sphere.indices.data(), GL_MAP_READ_BIT);
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

	glm::mat4 translation = glm::translate(glm::mat4(1.), glm::vec3(0., 0., -2.));
	glm::mat4 modelView = glm::rotate(translation, 45.f, glm::vec3(1., 0., 0.));
	glm::mat4 projection = glm::perspective(glm::radians(60.0), 16.0/9.0, 0.1, 1000.0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPointSize(20.0f);
	while (!window.shouldClose())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		shader.uniformMatrix4("projection", projection);
		shader.uniformMatrix4("view", window.camera.getView());

		auto trajectories = planetSystem.calculateTrajectories(trajectorySteps, 2);

		planetSystem.update(window.deltaTime);
		shader.uniformVector3("lightPosition", planetSystem.planets[0].position);
		shader.uniformVector3("lightColor", colors[0]);
		for (int i = 0; i < planetSystem.planets.size(); i++) {
			shader.uniformBool("useLighting", i != 0);
			shader.uniformVector3("planetColor", colors[i % planetSystem.planets.size()]);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, planetSystem.planets[i].position);
			model = glm::scale(model, glm::vec3(planetSystem.planets[i].radius));

			shader.uniformMatrix4("model", model);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);
			if (window.debugInformation.showTrajectories) {
				glNamedBufferSubData(trajectoryBuffer, 0, sizeof(glm::vec3) * trajectorySteps, trajectories[i].data());
				glBindVertexArray(trajectoryVAO);
				shader.uniformBool("useLighting", false);
				shader.uniformMatrix4("model", glm::mat4(1));
				glDrawArrays(GL_LINE_STRIP, 0, trajectories[i].size());
			}
		}

		window.runLoopFunctions();
	}
}

