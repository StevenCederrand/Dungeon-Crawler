#ifndef _MESH_H
#define _MESH_H
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
class Mesh {

public:
	Mesh();
	~Mesh();

	/* Setters */
	void setVao(const unsigned int& vao);
	void setNrOfIndices(const unsigned int& nrOfIndices);
	void setTextureID(const unsigned int& textureID);
	void setNormalID(const unsigned int& normalID);
	void setAmbientID(const unsigned int& ambientID);
	void setSpecularColor(const glm::vec3& specularColor);
	void setAmbientColor(const glm::vec3& ambientColor);
	void setDiffuseColor(const glm::vec3& diffuseColor);
	void setShininess(const float& shininess);
	void setBoundingBoxMinMax(std::vector<glm::vec3> maxMinVector);
	void setHasNormalMap(bool hasNormalMap);
	void setMaxMinValues(const glm::vec4& maxMinValues);

	/* Getters */
	const unsigned int& getVao() const;
	const unsigned int& getNrOfIndices() const;
	const unsigned int& getTextureID() const;
	const unsigned int& getNormalID() const;
	const unsigned int& getAmbientID() const;
	const glm::vec3& getSpecularColor() const;
	const glm::vec3& getAmbientColor() const;
	const glm::vec3& getdiffuseColor() const;
	const std::vector<glm::vec3>& getMinMaxVector() const;
	const float& getShininess() const;
	const glm::vec4& getMaxMinValues() const;
	const bool& hasNormalMap() const;
	const bool& hasAmbientMap() const;


private:
	unsigned int m_vao;
	unsigned int m_textureID;
	unsigned int m_normalID;
	unsigned int m_ambientID;
	unsigned int m_nrOfIndices;
	glm::vec3 m_specularColor;
	glm::vec3 m_ambientColor;
	glm::vec3 m_diffuseColor;
	std::vector<glm::vec3> m_minMaxVector;
	float m_shininess;
	glm::vec4 m_maxMinValues;
	bool m_hasNormalMap;
	bool m_hasAmbientMap;
	
};


#endif
