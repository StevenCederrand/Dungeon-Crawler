#include "Parser.h"
#include <fstream>
#include "Globals/Paths.h"
#include "System/Log.h"

#define CAPACITY 250000

Parser::Parser()
{
}

Parser::~Parser()
{
}

ParserData * Parser::loadFromObj(const std::string & filename)
{
	
	std::vector<GLfloat> tempVertexBuffer;
	tempVertexBuffer.reserve(CAPACITY);

	std::vector<GLfloat> tempUVBuffer;
	tempUVBuffer.reserve(CAPACITY);

	std::vector<GLfloat> tempNormalBuffer;
	tempNormalBuffer.reserve(CAPACITY);

	std::ifstream objFile(filename);
	if (!objFile.is_open())
	{
		LOG_ERROR("Could not find OBJ file " + filename);
		return nullptr;
	}

	ParserData* data = new ParserData();
	std::string line;
	std::string MTLfile = "";
	GLuint indexCount = 0;







	return nullptr;
}

std::vector<std::string> Parser::split(const std::string & line, const char splitter)
{

	return std::vector<std::string>();
}

void Parser::processFace(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint & indexCounter, 
	const std::vector<GLfloat>& tempVertices, const std::vector<GLfloat>& tempUvs, const std::vector<GLfloat>& tempNormals, 
	ParserData * parserData)
{


}
