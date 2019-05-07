#ifndef _HEALTHPLANE_h
#define _HEALTHPLANE_h
#include "GameObject.h"

class HealthPlane : public GameObject 
{
public:
	HealthPlane(Mesh* mesh, Type type);
	
	void update(float dt);
	Type getType();

private:
	Type m_type;

};

#endif // !_HEALTHPLANE_h
