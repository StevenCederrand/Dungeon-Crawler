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


	glm::vec3 m_position;
	float m_size;

	glm::mat4 m_modelMatrix;

	Player* m_player;
};

#endif
