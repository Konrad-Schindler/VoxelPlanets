#include <vector>
#include "Planet.hpp"
#include <glm/glm.hpp>

const float GRAVITATIONAL_CONSTANT = 6.6743e-2;

struct PlanetSystem {
	PlanetSystem();
	void addPlanet(glm::vec3 position, glm::vec3 velocity, float mass, float radius);
	void update(float dt);

	std::vector<Planet> planets;
	std::vector<std::vector<glm::vec3>> calculateTrajectories(int steps, float dt);

private:
	float computeGravitationalAcceleration(Planet from, Planet to);
};