#include "ProjectileManager.h"
#include <System/Log.h>
ProjectileManager::ProjectileManager(GLinit* glInit)
{
	m_player = nullptr;
	m_textureID = glInit->createTexture("GunFlare.png", true, true);
	m_size = 1.0f;
	m_vertex_buffer_data[0] = -m_size;
	m_vertex_buffer_data[1] = -m_size;
	m_vertex_buffer_data[2] =  0.0f;

	m_vertex_buffer_data[3] =  m_size;
	m_vertex_buffer_data[4] = -m_size;
	m_vertex_buffer_data[5] =  0.0f;

	m_vertex_buffer_data[6] = -m_size;
	m_vertex_buffer_data[7] =  m_size;
	m_vertex_buffer_data[8] =  0.0f;

	m_vertex_buffer_data[9] =  m_size;
	m_vertex_buffer_data[10] = m_size;
	m_vertex_buffer_data[11] = 0.0f;

	setupGraphicBuffers();
}

ProjectileManager::~ProjectileManager()
{
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_uvVBO);
	glDeleteBuffers(1, &m_centerVBO);
	glDeleteVertexArrays(1, &m_vao);

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		delete m_projectiles[i];
	}

	m_projectiles.clear();
}

void ProjectileManager::setPlayer(GameObject* player)
{
	m_player = dynamic_cast<Player*>(player);
}

void ProjectileManager::update(float dt)
{

	m_centerPosBuffer.clear();
	if (m_player != nullptr) {

		int vectorSize = m_projectiles.size();
		for (int i = vectorSize - 1; i >= 0; i--)
		{

			Projectile* proj = m_projectiles[i];
			proj->update(dt);

			if (proj->getAABB()->checkCollision(*m_player->getBoundingBoxes()[0]))
			{
				m_player->setHealth(m_player->getHealth() - proj->getDamage());
				delete proj;
				m_projectiles.erase(m_projectiles.begin() + i);
				continue;
			}

			if (proj->isAtDestination())
			{
				delete proj;
				m_projectiles.erase(m_projectiles.begin() + i);
				continue;
			}

			m_centerPosBuffer.emplace_back(proj->getPosition());

		}

		updateBuffers();
	}
}


void ProjectileManager::spawnProjectile(Projectile* projectile)
{
	m_projectiles.emplace_back(projectile);
}

const GLuint& ProjectileManager::getVAO() const
{
	return m_vao;
}

const GLuint& ProjectileManager::getTextureID()
{
	return m_textureID;
}

const size_t ProjectileManager::getNumberOfEnemyProjectiles() const
{
	return m_projectiles.size();
}

void ProjectileManager::setupGraphicBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_uvVBO);
	glGenBuffers(1, &m_centerVBO);

	glBindVertexArray(m_vao);

	// Buffer for vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for uv
	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_uv_buffer_data), m_uv_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	// Buffer for centerPositions
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PROJECTILE * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(NULL);

}

void ProjectileManager::updateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PROJECTILE * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerPosBuffer.size() * sizeof(glm::vec3), m_centerPosBuffer.data());

}
