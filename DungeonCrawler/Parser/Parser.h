#ifndef _PARSER_H
#define _PARSER_H
#include "ParserData.h"
#include <GLM/glm.hpp>
#include <vector>
#include <string>


class Parser {
public:
	Parser();
	~Parser();
	ParserData* loadFromObj(const std::string& filename);
	void cleanMemoryAllocated();

private:
	std::vector<std::string> split(const std::string& line, const char splitter);

	void parseMaterialFile(const std::string& filename, ParserData* parserData);
	void processFace(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint& indexCounter,
		const std::vector<glm::vec3>& tempVertices,
		const std::vector<glm::vec2>& tempUvs,
		const std::vector<glm::vec3>& tempNormals,
		ParserData* parserData);

private:
	std::vector<ParserData*> m_memoryTracker;
};

#endif