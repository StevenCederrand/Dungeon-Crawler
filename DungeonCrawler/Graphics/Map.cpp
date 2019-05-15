#include "Map.h"
#include "System/Input.h"
#include <GLM/gtx/transform.hpp>

Map::Map(GameObjectManager* gameManager)
{
	m_playerRoom = 0;
	m_gameObjectManager = gameManager;
	m_shouldRender = false;
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
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Map::update(float dt)
{
	std::vector<Room*> clearedRooms = m_gameObjectManager->getClearedRooms();
	int clearedRoomSize = clearedRooms.size();
	
	int mapRoomSize = m_rooms.size();
	for (size_t i = mapRoomSize; i < clearedRoomSize; i++)
	{
		m_rooms.emplace_back(clearedRooms.at(i)->getMaxMinValues());

	}

	if (Input::isKeyReleased(GLFW_KEY_M))
	{
		LOG_INFO("hello");
		m_shouldRender = !m_shouldRender;
	}

	Player* player = m_gameObjectManager->getPlayer();
	m_modelMatrix = glm::mat4(1.0f);

	//move the matrix to players position and then move it upp and closer to the camera
	m_modelMatrix = glm::translate(m_modelMatrix, player->getPlayerPosition());
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 8.0f, 2.0f));

	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

}


const std::vector<glm::vec4>& Map::getRoomCoordinates() const
{
	return m_rooms;
}

bool Map::getShouldRender() const
{
	return m_shouldRender;
}

const GLuint& Map::getVao() const
{
	return m_vao;
}

void Map::setShouldRender(bool shouldRender)
{
	m_shouldRender = shouldRender;
}

int Map::roomWithPlayer()
{
	glm::vec3 playerPosition = m_gameObjectManager->getPlayer()->getPlayerPosition();
	for (size_t i = 0; i < m_rooms.size(); i++)
	{
		if (playerPosition.x < m_rooms.at(i).x &&
			playerPosition.x > m_rooms.at(i).z &&
			playerPosition.z < m_rooms.at(i).y &&
			playerPosition.z > m_rooms.at(i).w) {
			m_playerRoom = i;
			
		}
	}
	return m_playerRoom;
}

glm::mat4 Map::getModelMatrix() const
{
	return m_modelMatrix;
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


