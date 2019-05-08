#ifndef _RAY_h
#define _RAY_h
#include <GLM/glm.hpp>

class Ray
{
public:
	glm::vec3 pos;
	glm::vec3 dir;

	Ray(glm::vec3 pos, glm::vec3 dir);
	
	glm::vec3 calcPoint(float t);


private:
	
};
#endif

inline Ray::Ray(glm::vec3 pos, glm::vec3 dir) 
{
	this->pos = pos;
	this->dir = dir;
}

inline glm::vec3 Ray::calcPoint(float t)
{
	return pos + (dir * t);
}