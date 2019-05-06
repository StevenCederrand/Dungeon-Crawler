#include "Map.h"

Map::Map()
{
	m_render = false;
	m_vao = 0;
	m_vbo = 0;
	GLfloat data[36] = {
		-0.5, 0.5, 0.0,
		-0.5, -0.5, 0.0,
		0.5, 0.5, 0.0,

		0.5, 0.5, 0.0,
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,

	};

	setupBuffers(data);
}

Map::~Map()
{
}

void Map::update(float dt, std::vector<GameObject*> gameObject)
{

	for (size_t i = 0; i < gameObject.size(); i++) {
		GameObject* object = gameObject[i];
		if (object->getType() == ROOM_EMPTY) {
			m_rooms.emplace_back(object->getMaxMinValues());
		}
	}



}

const std::vector<glm::vec4>& Map::getRoomCoordinates() const
{
	return m_rooms;
}

const GLuint& Map::getVao() const
{
	return m_vao;
}



void Map::setupBuffers(GLfloat data[])
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);
}


