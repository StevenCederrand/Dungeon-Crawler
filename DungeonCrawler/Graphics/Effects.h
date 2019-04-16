#ifndef _EFFECTS_H
#define _EFFECTS_H
#include <GLM/glm.hpp>
#include <vector>
#include <array>
#include <GL/glew.h>
class Effects
{
#define MAX_LASERS 100

public:
	Effects();
	~Effects();

	void update(float dt);

	void addLaser(const glm::vec3& startpos, const glm::vec3& endpos, float lifetime);
	const GLuint getVAO() const;
	unsigned int getNrOfAliveLasers() const;


private:

	GLfloat m_vertex_buffer_data[12] = {
	   -0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
	   -0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, -0.5f,
	};

	//--------- LASER  ---------
	struct LaserStruct {
		glm::vec3 centerpos;
		// first two values is the length of the two axis (x,z) 
		// and the other two is the scale ( how much of each length should be applied, depends on angle )
		glm::vec4 lengthsAndScales; 
		float currentLifetime; // Current life time
		float initialLifetime; // The life time it was given
	};

	std::array<LaserStruct, MAX_LASERS> m_lasers;
	unsigned int m_nrAliveLasers;

	// Used to render it in the renderer class (vao, vbo)
	GLuint m_vao;
	
	GLuint m_verticesVBO;
	GLuint m_centerVBO;
	GLuint m_lengthsAndScaleVBO;
	void setupGraphicBuffers();

	std::vector<glm::vec3> m_positionBuffer;
	std::vector<glm::vec4> m_lengthsAndScaleBuffer;

private:
	void updateBuffers();
	void swap(LaserStruct& l1, LaserStruct& l2);


};


#endif
