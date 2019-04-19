#ifndef _GLINIT_H
#define _GLINIT_H
#include "Parser/ParserData.h"
#include "Mesh.h"
#include <string>

class GLinit
{
public:

	GLinit();
	~GLinit();
	//If we're already have the mesh created then we return the pointer to that mesh.
	Mesh* createMesh(std::string name, ParserData* data);
	GLuint createTexture(std::string filename);
private:
	GLuint createAndBindVAO();
	void bindIndices(const std::vector<GLuint>& indices);
	void storeDataInAttributeList(const GLuint& attributeNumber, const GLuint& dataPerAttribute, const std::vector<glm::vec3>& dataVector);
	void storeDataInAttributeList(const GLuint& attributeNumber, const GLuint& dataPerAttribute, const std::vector<glm::vec2>& dataVector);
	
private:
	// Used for memory management
	std::vector<GLuint*> m_vaos;
	std::vector<GLuint*> m_vbos;
	std::vector<GLuint*> m_texIDs;

};

#endif
