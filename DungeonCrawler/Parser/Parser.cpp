#include "Parser.h"
#include <fstream>
#include <sstream>
#include "Globals/Paths.h"
#include "System/Log.h"

#define CAPACITY 5000

Parser::Parser()
{
}

Parser::~Parser()
{
	cleanMemoryAllocated();
}

ParserData * Parser::loadFromObj(const std::string & filename)
{
	
	std::vector<glm::vec3> tempVertexBuffer;
	tempVertexBuffer.reserve(CAPACITY);

	std::vector<glm::vec2> tempUVBuffer;
	tempUVBuffer.reserve(CAPACITY);

	std::vector<glm::vec3> tempNormalBuffer;
	tempNormalBuffer.reserve(CAPACITY);

	std::ifstream objFile(OBJFilePath + filename);
	if (!objFile.is_open())
	{
		LOG_ERROR("Could not find OBJ file " + filename + "\nFull path: " + OBJFilePath + filename);
		return nullptr;
	}


	ParserData* data = new ParserData(CAPACITY);
	std::string line;
	std::string MTLfile = "";
	GLuint indexCount = 0;
	bool isParsingCollider = false;

	glm::vec3 min = glm::vec3(100000);
	glm::vec3 max = glm::vec3(-100000);


	while (std::getline(objFile, line))
	{
		std::vector<std::string> attribs = split(line, ' ');
		this->stringClean(attribs);

		if (attribs[0] == "#") {

			if (attribs[1] == "object" && attribs[2] == "collision")
			{
				isParsingCollider = true;
			}
			continue;
		}

		if (attribs.size() == 0)
			continue;

		if (attribs[0] == "mtllib")
		{
			MTLfile = attribs[1];
		}
		else if (attribs[0] == "v")
		{
			if (!isParsingCollider) {
				glm::vec3 vert = glm::vec3(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
				tempVertexBuffer.emplace_back(vert);
			}
			else
			{
				// Is parsing the collider so only check for the min x,y,z and max x,y,z cooridnates!
				float x = std::stof(attribs[1]);
				float y = std::stof(attribs[2]);
				float z = std::stof(attribs[3]);

				if (min.x > x) min.x = x;
				if (min.y > y) min.y = y;
				if (min.z > z) min.z = y;

				if (max.x < x) max.x = x;
				if (max.y < y) max.y = y;
				if (max.z < z) max.z = z;

			}

		}
		else if (attribs[0] == "vt" && !isParsingCollider)
		{
			glm::vec2 vert = glm::vec2(std::stof(attribs[1]), std::stof(attribs[2]));
			tempUVBuffer.emplace_back(vert);
		}
		else if (attribs[0] == "vn" && !isParsingCollider)
		{
			glm::vec3 vert = glm::vec3(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
			tempNormalBuffer.emplace_back(vert);
		}
		else if (attribs[0] == "f" && !isParsingCollider)
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

	// Parse the material 
	parseMaterialFile(MTLfile, data);

	m_memoryTracker.emplace_back(data);
	
	return data;
}

void Parser::cleanMemoryAllocated()
{
	for (size_t i = 0; i < m_memoryTracker.size(); i++)
	{
		delete m_memoryTracker[i];
	}

	m_memoryTracker.clear();
}

void Parser::parseSSO(const std::string filename) {
	std::string line;
	std::ifstream ssoFile(SoundPath + filename);

	if (!ssoFile.is_open()) {
		LOG_ERROR("ERROR OPENING SSO FILE");
	}

	int lines = 0;
	while (std::getline(ssoFile, line)) {
		//Ignore comment and empty line
		if (line[0] == '#' || line == "") {
			continue;
		}
		
		size_t space = line.find(' ');
		std::string key = line.substr(0, space);
		std::string filename = line.substr(++space, line.size());
		AudioEngine::loadSound(filename, key);

		LOG_INFO(key + " " + filename);
	}
	LOG_INFO(lines);
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

int Parser::getFaceIndexIfExist(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint indexCounter)
{
	for (size_t i = 0; i < m_faces.size(); i++)
	{
		OBJFace& face = m_faces[i];

		if (face.vertexIndex == vertexIndex && face.uvIndex == uvIndex && face.normalIndex == normalIndex)
		{
			// It exist so return the index number
			return face.index;
		}
	}


	// If it gets here then it doesn't exist so add it to the list of faces
	OBJFace face;
	face.index = indexCounter;
	face.vertexIndex = vertexIndex;
	face.uvIndex = uvIndex;
	face.normalIndex = normalIndex;

	m_faces.emplace_back(face);

	return -1;
}

void Parser::processFace(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint & indexCounter, 
	const std::vector<glm::vec3>& tempVertices, const std::vector<glm::vec2>& tempUvs, const std::vector<glm::vec3>& tempNormals,
	ParserData * parserData)
{
	int vertexStartPos = (vertexIndex - 1);
	int uvStartPos = (uvIndex - 1);
	int normalStartPos = (normalIndex - 1);

	int index = getFaceIndexIfExist(vertexIndex, uvIndex, normalIndex, indexCounter);

	// It exists so just add the index
	if (index != -1)
	{
		parserData->addIndex(index);
		return;
	}


	parserData->addIndex(indexCounter++);
	parserData->addVertex(tempVertices[vertexStartPos]);
	parserData->addUV(tempUvs[uvStartPos]);
	parserData->addNormal(tempNormals[normalStartPos]);

	glm::vec3 pos = tempVertices[vertexStartPos];
	//LOG_TRACE(std::to_string(pos.x) + ", " +  std::to_string(pos.y) + ", " + std::to_string(pos.z));
	//glm::vec3 pos = tempVertices[vertexStartPos];
	
}

void Parser::parseMaterialFile(const std::string& filename, ParserData* parserData)
{
	std::ifstream mtlFile(OBJFilePath + filename);
	if (!mtlFile.is_open())
	{
		LOG_ERROR("Could not find Material file " + filename);
		return;
	}

	std::string line;
	while (std::getline(mtlFile, line))
	{

		std::vector<std::string> attribs = split(line, ' ');
		if (attribs.size() == 0)
			continue;

		if (attribs[0] == "Ka")
		{
			parserData->setAmbientColor(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
		}
		else if (attribs[0] == "Kd")
		{
			parserData->setDiffuseColor(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
		}
		else if (attribs[0] == "Ks")
		{
			parserData->setSpecularColor(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
		}
		else if (attribs[0] == "Ns" || attribs[0] == "Ni")
		{
			parserData->setShininess(std::stof(attribs[1]));
		}
		else if (attribs[0] == "map_Kd")
		{
			parserData->setTextureFilename(TexturePath + attribs[1]);
		}

	}

	mtlFile.close();
}

void Parser::stringClean(std::vector<std::string>& attribs) {
	std::vector<std::string> newattribs;
	for (size_t i = 0; i < attribs.size(); i++){
		if (attribs.at(i) == "")
			continue;
		newattribs.emplace_back(attribs.at(i));
	}
	attribs = newattribs;
}