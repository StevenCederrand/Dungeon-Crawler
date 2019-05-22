#ifndef _ROOM_H
#define _ROOM_H
#include "Player.h"
#include <AI/GridSystem.h>

class Room : public GameObject
{
public:
	Room(Mesh* mesh, Type type, GameObject* playerObject ,const glm::vec3& position = glm::vec3(0));
	~Room();

	void update(float dt);
	bool intersection(glm::vec3 position);

	GridSystem* getGrid() const;

	const glm::vec2& getCentrePosition() const;
	void resetMaxMinValues();
	
private:
	void setupMaxMinValues();
	void calculateCentrePosition();
private:
	glm::vec2 centrePosition;
	GridSystem* m_gridsystem;
	Type m_type;
	GameObject* m_playerObject;
	bool m_reset;
};


#endif
