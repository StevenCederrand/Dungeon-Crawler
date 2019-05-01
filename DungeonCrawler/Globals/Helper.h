#ifndef HELPER_h
#define HELPER_h
#include <GLM/glm.hpp>
#include <string>

static std::string vec4ToString(const glm::vec4& vec) {
	std::string x = "X: " + std::to_string(vec.x) + " Y: " 
		+ std::to_string(vec.y) + " Z: " + std::to_string(vec.z) + 
		" W: " + std::to_string(vec.w) + "\n";
	return x;
}
static std::string vec3ToString(const glm::vec3& vec) {
	std::string x = "X: " + std::to_string(vec.x) + " Y: "
		+ std::to_string(vec.y) + " Z: " + std::to_string(vec.z) + "\n";
	return x;
}


#endif