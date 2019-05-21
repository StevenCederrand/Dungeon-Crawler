#include "ProjectileManager.h"
#include <System/Log.h>
#include <Utility/Randomizer.h>
#include <Audio/AudioEngine.h>

ProjectileManager::ProjectileManager(GLinit* glInit, Effects* effects, const std::string& textureNameWithoutPath)
{
	m_player = nullptr;
	m_effects = effects;
	m_textureID = glInit->createTexture(textureNameWithoutPath, true, true);
	m_size = 0.5f;
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
	m_col = glm::vec4(1.0f);
	
	m_frames = 0;
	m_isAnmiated = false;
	
	setupGraphicBuffers();

}

ProjectileManager::ProjectileManager(GLinit* glInit, Effects* effects, const std::string& SpriteSheetNameWithoutPath, int pixelWidth, int pixelHeight, int frames, float animDelay)
{
	m_player = nullptr;
	m_effects = effects;
	m_textureID = glInit->createTexture(SpriteSheetNameWithoutPath, true, true);
	m_size = 0.5f;
	m_vertex_buffer_data[0] = -m_size;
	m_vertex_buffer_data[1] = -m_size;
	m_vertex_buffer_data[2] = 0.0f;

	m_vertex_buffer_data[3] = m_size;
	m_vertex_buffer_data[4] = -m_size;
	m_vertex_buffer_data[5] = 0.0f;

	m_vertex_buffer_data[6] = -m_size;
	m_vertex_buffer_data[7] = m_size;
	m_vertex_buffer_data[8] = 0.0f;

	m_vertex_buffer_data[9] = m_size;
	m_vertex_buffer_data[10] = m_size;
	m_vertex_buffer_data[11] = 0.0f;

	m_isAnmiated = true;
	m_pxWidth = pixelWidth;
	m_pxHeight = pixelHeight;
	m_frames = frames;
	m_animDelay = animDelay;
	m_col = glm::vec4(1.0f);

	setupAnimatedGraphicBuffers();

}

ProjectileManager::~ProjectileManager()
{
	glDeleteBuffers(1, &m_verticesVBO);
	glDeleteBuffers(1, &m_uvVBO);
	glDeleteBuffers(1, &m_centerVBO);
	glDeleteBuffers(1, &m_colorVBO);
	glDeleteBuffers(1, &m_anim_currentIndexVBO);
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
	m_animationIndices.clear();

	if (m_player != nullptr) {

		int vectorSize = m_projectiles.size();
		for (int i = vectorSize - 1; i >= 0; i--)
		{

			Projectile* proj = m_projectiles[i];
			proj->update(dt);
			//If the projectile collides with the player
			if (proj->getAABB()->checkCollision(*m_player->getBoundingBoxes()[0]))
			{
				for (int i = 0; i < 10; i++) {
					m_effects->addParticles("BloodEmitter", m_player->getPosition(),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 100.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 100.0f),
						0.75f,
						1);

					m_effects->addParticles("ProjectileExplosionEmitter", m_player->getPosition(),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 10.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 10.0f),
						0.50f,
						1);
				}
				AudioEngine::play("pl_ranged_damage_taken", 1.0f),

				m_player->setHealth(m_player->getHealth() - proj->getDamage());
				delete proj;
				m_projectiles.erase(m_projectiles.begin() + i);
				continue;
			}

			if (proj->isAtDestination())
			{
				for (int i = 0; i < 10; i++) {
					
					m_effects->addParticles("ProjectileExplosionEmitter", proj->getPosition(),
						glm::vec3(Randomizer::single(-100.0f, 100.0f) / 50.0f, 0.0f, Randomizer::single(-100.0f, 100.0f) / 50.0f),
						0.75f,
						1);

				}

				delete proj;
				m_projectiles.erase(m_projectiles.begin() + i);
				continue;
			}

			m_centerPosBuffer.emplace_back(proj->getPosition());
			if(m_isAnmiated)
				addAnimatedIndicesToVec(proj);
		}
		if (m_isAnmiated)
			updateAnimatedBuffers();
		else
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

const int ProjectileManager::getNumberOfAnimationFrames() const
{
	return m_frames;
}

void ProjectileManager::setupGraphicBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_uvVBO);
	glGenBuffers(1, &m_centerVBO);
	glGenBuffers(1, &m_colorVBO);
	glGenBuffers(1, &m_anim_currentIndexVBO);

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

	// Buffer for color
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), &m_col, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(3, MAX_PROJECTILE);
	
	// Animation index
	glBindBuffer(GL_ARRAY_BUFFER, m_anim_currentIndexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), &m_indx, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(4, MAX_PROJECTILE);

	glBindVertexArray(NULL);

}

void ProjectileManager::updateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PROJECTILE * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerPosBuffer.size() * sizeof(glm::vec3), m_centerPosBuffer.data());

}

void ProjectileManager::setupAnimatedGraphicBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_verticesVBO);
	glGenBuffers(1, &m_uvVBO);
	glGenBuffers(1, &m_centerVBO);
	glGenBuffers(1, &m_colorVBO);
	glGenBuffers(1, &m_anim_currentIndexVBO);
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

	// Buffer for color
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), &m_col, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(3, MAX_PROJECTILE);

	// Animation index
	glBindBuffer(GL_ARRAY_BUFFER, m_anim_currentIndexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(NULL);
}

void ProjectileManager::updateAnimatedBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_centerVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PROJECTILE * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerPosBuffer.size() * sizeof(glm::vec3), m_centerPosBuffer.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_anim_currentIndexVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PROJECTILE * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_animationIndices.size() * sizeof(float), m_animationIndices.data());
}

void ProjectileManager::addAnimatedIndicesToVec(Projectile* proj)
{
	int projCurrentAnimIndex = proj->getInternalAnimIndex();

	if (proj->getInternalAnimTimer() >= m_animDelay)
	{
		proj->resetInternalAnimTimer();
		
		projCurrentAnimIndex++;

		if (projCurrentAnimIndex > m_frames) {
			projCurrentAnimIndex = 0;
		}
		proj->setInternalAnimIndex(projCurrentAnimIndex);
			
	}

	m_animationIndices.emplace_back((float)projCurrentAnimIndex);
}
