#ifndef _PARSER_H
#define _PARSER_H
#include "ParserData.h"
#include <GL/glew.h>
#include <vector>
#include <string>


class Parser {
public:
	Parser();
	~Parser();
	ParserData* loadFromObj(const std::string& filename);

private:
	std::vector<std::string> split(const std::string& line, const char splitter);

	void processFace(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint& indexCounter,
		const std::vector<GLfloat>& tempVertices,
		const std::vector<GLfloat>& tempUvs,
		const std::vector<GLfloat>& tempNormals,
		ParserData* parserData);

};

#endif
