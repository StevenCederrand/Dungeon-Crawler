#ifndef _PARSERDATA_H
#define _PARSERDATA_H
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include <string>


class ParserData {

public:
	ParserData(int startCapacity);
	~ParserData();

	/* Setters */
	void addIndex(GLuint index);
	void addVertex(glm::vec3 vertex);
	void addUV(glm::vec2 uv);
	void addNormal(glm::vec3 normal);
	
	void setTextureFilename(std::string filename);
	void setNormalMapName(std::string filename);

	void setFilename(std::string filename);
	void setAmbientMapName(std::string filename);

	void setDiffuseColor(GLfloat r, GLfloat g, GLfloat b);
	void setSpecularColor(GLfloat r, GLfloat g, GLfloat b);
	void setAmbientColor(GLfloat r, GLfloat g, GLfloat b);
	void setShininess(GLfloat factor);
	void setBoundingBox(const std::vector<glm::vec3>& maxMinVector);
	void setMaxMinValues(const glm::vec4& maxMinValues);

	void setNormalMapStrength(float str);

	/* Getters */
	const std::vector<GLuint>& getIndices() const;
	const std::vector<glm::vec3>& getVertices() const;
	const std::vector<glm::vec2>& getUvs() const;
	const std::vector<glm::vec3>& getNormals() const;

	const std::string& getTextureFilename() const;
	const std::string& getNormalMapName() const;

	const std::string& getFilename() const;
	const std::string& getAmbientMapName() const;

	const glm::vec3& getDiffuseColor() const;
	const glm::vec3& getSpecularColor() const;
	const glm::vec3& getAmbientColor() const;
	const GLfloat& getShininess() const;
	const std::vector<glm::vec3>& getMaxMinVector() const;
	const glm::vec4& getMaxMinValues() const;

	const GLfloat& getNormalMapStrength() const;
	const bool hasNormalMap();
	const bool hasAmbientMap();

private:
	std::vector<GLuint> m_indices;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;

	std::string m_textureFilename;
	std::string m_normalMapName;

	std::string m_filename;
	std::string m_ambientMapName;

	glm::vec3 m_diffuseColor;
	glm::vec3 m_specularColor;
	glm::vec3 m_ambientColor;

	glm::vec4 m_maxMinValues;

	GLfloat m_shininess;
	GLfloat m_normalMapStrength;

	// Collision coordinates ( layout in the vector)
	std::vector<glm::vec3> m_maxMinVector;


};


#endif
