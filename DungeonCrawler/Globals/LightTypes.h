#ifndef LIGHTTYPES_h
#define LIGHTTYPES_h
#include <GLM/glm.hpp>

struct Spotlight {
	glm::vec3 position;
	glm::vec3 direction;
	float radius;
	float outerRadius;
};

struct Light {
	glm::vec4 position;
	//The alpha is the light radius
	glm::vec4 color;
};



#endif