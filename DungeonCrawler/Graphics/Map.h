#ifndef _MAP_H
#define _MAP_H
#include "../GameObjects/GameObject.h"


class Map
{
public:
	Map();
	~Map();

	void update(float dt, std::vector<GameObject*> gameObject);

	const std::vector<glm::vec4> &getRoomCoordinates()const;
	bool shouldRender();
	const GLuint &getVao()const;
private: 
	
	void setupBuffers(GLfloat data[]);

private:

	GLuint m_vao;
	GLuint m_vbo;
	
	std::vector<glm::vec4> m_rooms;
	bool m_render;

};



#endif // !_MAP_H

