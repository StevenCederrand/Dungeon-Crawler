#ifndef _LIGHT_SPHERE_H
#define _LIGHT_SPHERE_H
#include "GameObject.h"

class LightSphere : public GameObject {

public:

	LightSphere(Mesh* mesh, const glm::vec3& position);
	virtual ~LightSphere();

	void update(float dt);

private:

};

#endif
