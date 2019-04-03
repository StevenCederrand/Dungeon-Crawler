#ifndef _BOX_H
#define _BOX_H
#include "GameObject.h"

class Box : public GameObject {
public:
	Box(Mesh* mesh);
	void update(float dt);

private:


};


#endif
