#ifndef _MESH_H
#define _MESH_H
#include <GL/glew.h>
#include <GLM/glm.hpp>

class Mesh {

public:
	Mesh();
	~Mesh();

	/* Setters */
	void setVao(const unsigned int& vao);
	void setNrOfIndices(const unsigned int& nrOfIndices);
	void setTextureID(const unsigned int& textureID);
	void setSpecularColor(const glm::vec3& specularColor);
	void setAmbientColor(const glm::vec3& ambientColor);
	void setDiffuseColor(const glm::vec3& diffuseColor);
	void setShininess(const float& shininess);
	void setBoundingBoxMinMax(const glm::vec3& min, const glm::vec3& max);

	/* Getters */
	const unsigned int& getVao() const;
	const unsigned int& getNrOfIndices() const;
	const unsigned int& getTextureID() const;
	const glm::vec3& getSpecularColor() const;
	const glm::vec3& getAmbientColor() const;
	const glm::vec3& getdiffuseColor() const;
	const glm::vec3& getBoundingBoxMin() const;
	const glm::vec3& getBoundingBoxMax() const;
	const float& getShininess() const;
	

private:
	unsigned int m_vao;
	unsigned int m_textureID;
	unsigned int m_nrOfIndices;
	glm::vec3 m_specularColor;
	glm::vec3 m_ambientColor;
	glm::vec3 m_diffuseColor;
	glm::vec3 m_boundingBoxMin;
	glm::vec3 m_boundingBoxMax;
	float m_shininess;

	


};


#endif
