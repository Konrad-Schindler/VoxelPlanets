#include "PlanetSystem.hpp"

PlanetSystem::PlanetSystem()
{
}

void PlanetSystem::addPlanet(glm::vec3 position, glm::vec3 velocity, float mass, float radius)
{
	Planet planet(position, velocity, mass, radius);
	planets.push_back(planet);
}

void PlanetSystem::update(float dt)
{
	for (Planet& planet : planets) {
		glm::vec3 accelerationAcc = glm::vec3(0.0f);
		for (Planet& otherPlanet : planets) {
			if (&planet != &otherPlanet) {
				float acceleration = computeGravitationalAcceleration(planet, otherPlanet);
				glm::vec3 direction = glm::normalize(otherPlanet.position - planet.position);
				accelerationAcc += direction * acceleration;
			}
		}
		planet.acceleration = accelerationAcc;
		planet.update(dt);
	}
}

std::vector<std::vector<glm::vec3>> PlanetSystem::calculateTrajectories(int steps, float dt)
{
	std::vector<std::vector<glm::vec3>> trajectories;
	for (int i = 0; i < planets.size(); i++) {
		trajectories.push_back(std::vector<glm::vec3>());
	}
	auto planetsCopy = planets;
	for (int s = 0; s < steps; s++) {
		for (int i = 0; i < planets.size(); i++) {
			glm::vec3 accelerationAcc = glm::vec3(0.0f);
			for (Planet& otherPlanet : planets) {
				if (&planets[i] != &otherPlanet) {
					float acceleration = computeGravitationalAcceleration(planets[i], otherPlanet);
					glm::vec3 direction = glm::normalize(otherPlanet.position - planets[i].position);
					accelerationAcc += direction * acceleration;
				}
			}
			planets[i].acceleration = accelerationAcc;
			planets[i].update(dt);
			trajectories[i].push_back(planets[i].position);
		}
	}
	planets = planetsCopy;
	return trajectories;
}


float PlanetSystem::computeGravitationalAcceleration(Planet from, Planet to)
{
	glm::vec3 difference = to.position - from.position;
	float distanceSquared = glm::dot(difference, difference);
	return GRAVITATIONAL_CONSTANT * to.mass / distanceSquared;
}
