#ifndef _ROOM_H
#define _ROOM_H
#include "GameObject.h"

class Room : public GameObject
{
public:
	Room(Mesh* mesh, const glm::vec3& position = glm::vec3(0));
	~Room();

	void update(float dt);


private:



};


#endif
