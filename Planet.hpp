#include <glm/vec3.hpp>
#include <vector>

struct Planet {
	Planet(glm::vec3 position, glm::vec3 velocity, float mass, float radius);
	void update(float dt);
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;
	float radius;
};