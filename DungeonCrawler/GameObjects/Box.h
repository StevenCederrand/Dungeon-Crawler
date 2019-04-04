#ifndef _BOX_H
#define _BOX_H
#include "GameObject.h"

class Box : public GameObject {
public:
	Box(Mesh* mesh, const glm::vec3& position = glm::vec3(0.f));
	void update(float dt);

private:


};


#endif
