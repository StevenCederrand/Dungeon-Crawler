#ifndef _PLAYERHEALTHBAR_H
#define _PLAYERHEALTHBAR_H
#include <GLM/glm.hpp>
#include <Graphics/GLinit.h>
#include <GameObjects/Player.h>

class PlayerHealthBar {
public:
	PlayerHealthBar(GLinit* glInit, Player* player);
	~PlayerHealthBar();

	void update(float dt);
	
	const glm::mat4& getModelMatrix() const;
	const GLuint& getVAO() const;
	const GLuint& getTextureID() const;

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_currentTexID;

	GLuint m_hpbar_1;
	GLuint m_hpbar_2;
	GLuint m_hpbar_3;
	GLuint m_hpbar_4;
	GLuint m_hpbar_5;

	glm::vec3 m_position;
	float m_size;
	float m_healthStageValue;
	float m_playerMaxHealth;

	glm::mat4 m_modelMatrix;

	Player* m_player;
};

#endif
