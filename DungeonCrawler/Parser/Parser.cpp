#include "Parser.h"
#include <fstream>
#include <sstream>
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
	
	std::vector<glm::vec3> tempVertexBuffer;
	tempVertexBuffer.reserve(CAPACITY);

	std::vector<glm::vec2> tempUVBuffer;
	tempUVBuffer.reserve(CAPACITY);

	std::vector<glm::vec3> tempNormalBuffer;
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

	while (std::getline(objFile, line))
	{
		std::vector<std::string> attribs = split(line, ' ');

		if (attribs.size() == 0)
			continue;

		if (attribs[0] == "mtllib")
		{
			MTLfile = attribs[1];
		}
		else if (attribs[0] == "v")
		{
			for (int i = 1; i < 4; i++)
				tempVertexBuffer.emplace_back(std::stof(attribs[i]));

		}
		else if (attribs[0] == "vt")
		{
			for (int i = 1; i < 3; i++)
				tempUVBuffer.emplace_back(std::stof(attribs[i]));
		}
		else if (attribs[0] == "vn")
		{
			for (int i = 1; i < 4; i++)
				tempNormalBuffer.emplace_back(std::stof(attribs[i]));

		}
		else if (attribs[0] == "f")
		{
			std::vector<std::string> attributes = split(line, ' ');
			for (int i = 1; i < attributes.size(); i++)
			{
				std::vector<std::string> currFace = split(attributes[i], '/');
				GLuint x = static_cast<GLuint>(std::stoi(currFace[0]));
				GLuint y = static_cast<GLuint>(std::stoi(currFace[1]));
				GLuint z = static_cast<GLuint>(std::stoi(currFace[2]));
				processFace(x, y, z, indexCount, tempVertexBuffer, tempUVBuffer, tempNormalBuffer, data);
			}
		}


	}

	objFile.close();
	tempVertexBuffer.clear();
	tempUVBuffer.clear();
	tempNormalBuffer.clear();


	return nullptr;
}

std::vector<std::string> Parser::split(const std::string & line, const char splitter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(line);
	
	while (std::getline(tokenStream, token, splitter))
	{
		tokens.emplace_back(token);
	}

	return tokens;
}

void Parser::processFace(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint & indexCounter, 
	const std::vector<glm::vec3>& tempVertices, const std::vector<glm::vec2>& tempUvs, const std::vector<glm::vec3>& tempNormals,
	ParserData * parserData)
{
	int vertexStartPos = (vertexIndex - 1) * 3;
	int uvStartPos = (uvIndex - 1) * 2;
	int normalStartPos = (normalIndex - 1) * 3;

	/*parserData->addIndices(indexCounter++);
	parserData->addVertex(vertices[vertexStartPos], vertices[vertexStartPos + 1], vertices[vertexStartPos + 2]);
	parserData->addUV(uvs[uvStartPos], uvs[uvStartPos + 1]);
	parserData->addNormal(normals[normalStartPos], normals[normalStartPos + 1], normals[normalStartPos + 2]);*/

}
