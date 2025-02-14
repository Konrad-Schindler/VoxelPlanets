#include "Planet.hpp"

Planet::Planet(glm::vec3 position, glm::vec3 velocity, float mass, float radius) : position(position), velocity(velocity), mass(mass), radius(radius)
{
	acceleration = glm::vec3(0.0f);
}

void Planet::update(float dt)
{
	velocity = velocity + acceleration * dt;
	position = position + velocity * dt;
}
