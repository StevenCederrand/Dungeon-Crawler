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
	void createMesh(std::string name, ParserData* data);

private:
	GLuint createAndBindVAO();
	void bindIndices(const std::vector<GLuint>& indices);
	void storeDataInAttributeList(const GLuint& attributeNumber, const GLuint& dataPerAttribute, GLuint bufferSize, const GLvoid * data);
	GLuint createTexture(std::string filename);

private:
	// Used for memory management
	std::vector<GLuint*> m_vaos;
	std::vector<GLuint*> m_vbos;
	std::vector<GLuint*> m_texIDs;

};

#endif
