#ifndef _PROJECTILEMANAGER_H
#define _PROJECTILEMANAGER_H
#include "Projectile.h"
#include <GameObjects/Player.h>
#include <Graphics/GLinit.h>
#define MAX_PROJECTILE 50

class ProjectileManager {
public:
	ProjectileManager(GLinit* glInit);
	~ProjectileManager();

	void setPlayer(GameObject* player);
	void update(float dt);
	void render();

	void spawnProjectile(Projectile* projectile);

	const GLuint& getVAO() const;
	const GLuint& getTextureID();
	const size_t getNumberOfEnemyProjectiles() const;
private:
	GLuint m_vao;
	GLuint m_verticesVBO;
	GLuint m_uvVBO;
	GLuint m_centerVBO;
	GLuint m_textureID;
	GLfloat m_vertex_buffer_data[12];
	GLfloat m_uv_buffer_data[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	void setupGraphicBuffers();
	void updateBuffers();
private:
	std::vector<Projectile*> m_projectiles;
	std::vector<glm::vec3> m_centerPosBuffer;
	Player* m_player;
	int m_size;
};

#endif
