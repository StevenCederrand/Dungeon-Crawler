#ifndef _PROJECTILEMANAGER_H
#define _PROJECTILEMANAGER_H
#include "Projectile.h"
#include <GameObjects/Player.h>
#include <Graphics/GLinit.h>
#include <Graphics/Effects.h>

#define MAX_PROJECTILE 50

class ProjectileManager {
public:
	ProjectileManager(GLinit* glInit, Effects* effects, const std::string& textureNameWithoutPath);
	ProjectileManager(GLinit* glInit, Effects* effects, const std::string& SpriteSheetNameWithoutPath, int pixelWidth, int pixelHeight, int frames, float animDelay);
	~ProjectileManager();

	void setPlayer(GameObject* player);
	void update(float dt);
	
	void spawnProjectile(Projectile* projectile);

	const GLuint& getVAO() const;
	const GLuint& getTextureID();
	const size_t getNumberOfEnemyProjectiles() const;
	const int getNumberOfAnimationFrames() const;
private:
	GLuint m_vao;
	GLuint m_verticesVBO;
	GLuint m_uvVBO;
	GLuint m_centerVBO;
	GLuint m_textureID;
	GLuint m_colorVBO;
	GLuint m_anim_currentIndexVBO;

	GLfloat m_vertex_buffer_data[12];
	
	// Used if projectiles is not going to be animated
	GLfloat m_uv_buffer_data[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	void setupGraphicBuffers();
	void updateBuffers();


	std::vector<float> m_animationIndices;
	void setupAnimatedGraphicBuffers();
	void updateAnimatedBuffers();
	void addAnimatedIndicesToVec(Projectile* proj);

private:
	std::vector<Projectile*> m_projectiles;
	std::vector<glm::vec3> m_centerPosBuffer;
	glm::vec4 m_col;
	Player* m_player;
	Effects* m_effects;
	float m_size;

	// Animation props
	float m_animDelay;
	int m_pxWidth;
	int m_pxHeight;
	int m_frames;
	bool m_isAnmiated;
	float m_indx = 0;
};

#endif
