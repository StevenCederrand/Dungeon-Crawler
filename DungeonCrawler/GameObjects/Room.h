#ifndef _ROOM_H
#define _ROOM_H
#include "GameObject.h"

class Room : public GameObject
{
public:
	Room(Mesh* mesh, Type type, const glm::vec3& position = glm::vec3(0));
	~Room();

	void update(float dt);
	bool intersection(glm::vec3 position);

	void setupMaxMinValues();
private:
private:

	Type m_type;
};


#endif
