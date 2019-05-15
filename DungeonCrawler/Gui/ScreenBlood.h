#ifndef _SCREENBLOOD_H
#define _SCREENBLOOD_H
#include <GLM/glm.hpp>
#include <Graphics/GLinit.h>
#include <GameObjects/Player.h>


class ScreenBlood {
public:
	ScreenBlood(GLinit* glInit, Player* player);
	~ScreenBlood();

	void update(float dt);

	const glm::mat4& getModelMatrix() const;
	const GLuint& getVAO() const;
	const GLuint& getTextureID() const;
	const bool& shouldRender() const;
	const float& getAlpha() const;
private:
	void setupBuffers(GLfloat data[]);
private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_texturID;

	glm::mat4 m_modelMatrix;

	Player* m_player;
	float m_savedHealth;
	bool m_takenDamage;
	float m_alphaValue;
};



#endif // !_SCREENBLOOD_H

