#ifndef LIGHTTYPES_h
#define LIGHTTYPES_h
#include <GLM/glm.hpp>

struct Spotlight {
	glm::vec3 position;
	glm::vec3 direction;
	float radius;
};

struct Light {
	glm::vec4 position;
	glm::vec4 color;
};



#endif