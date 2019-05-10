#ifndef _MAP_H
#define _MAP_H
#include "../GameObjects/GameObject.h"
#include "../GameObjects/GameObjectManager.h"


class Map
{
public:
	Map(GameObjectManager* gameManager);
	~Map();

	void update(float dt);

	const std::vector<glm::vec4> &getRoomCoordinates()const;
	bool getShouldRender()const;
	const GLuint &getVao()const;

	void setShouldRender(bool shouldRender);
	int roomWithPlayer(); //return which room in the vector the player is in
	glm::mat4 getModelMatrix()const;
private: 
	
	void setupBuffers(GLfloat data[]);

private:
	int m_playerRoom;

	GLuint m_vao;
	GLuint m_vbo;
	
	std::vector<glm::vec4> m_rooms;
	bool m_shouldRender;
	GameObjectManager* m_gameObjectManager;
	glm::mat4 m_modelMatrix;

};



#endif // !_MAP_H

