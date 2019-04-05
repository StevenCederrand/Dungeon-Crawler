#ifndef _PARSER_H
#define _PARSER_H
#include "ParserData.h"
#include <GLM/glm.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include "../Audio/AudioEngine.h"

class Parser {
public:
	Parser();
	~Parser();
	ParserData* loadFromObj(const std::string& filename);
	//Parse SSO files
	void parseSSO(const std::string filename);
	void cleanMemoryAllocated();

private:
	std::vector<std::string> split(const std::string& line, const char splitter);

	void parseMaterialFile(const std::string& filename, ParserData* parserData);
	void processFace(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint& indexCounter,
		const std::vector<glm::vec3>& tempVertices,
		const std::vector<glm::vec2>& tempUvs,
		const std::vector<glm::vec3>& tempNormals,
		ParserData* parserData);
	void stringClean(std::vector<std::string>& attribs);

	int getFaceIndexIfExist(GLuint vertexIndex, GLuint uvIndex, GLuint normalIndex, GLuint indexCounter);

	void writeToBinary(ParserData* data, const std::string& filename);
	void writeBinaryVecInt(std::ofstream& binaryFile, std::vector<GLuint> vector);
	void writeBinaryVecVec3(std::ofstream& binaryFile, std::vector<glm::vec3> vector);
	void writeBinaryVecVec2(std::ofstream& binaryFile, std::vector<glm::vec2> vector);
	void writeBinaryString(std::ofstream& binaryFile, std::string string);
	void writeBinaryVec3(std::ofstream& binaryFile, glm::vec3 vector);
	void writeBinaryVec2(std::ofstream& binaryFile, glm::vec2 vector);
	void writeBinaryFloat(std::ofstream& binaryFile, GLfloat floatValue);
	
	ParserData* loadFromBinary(const std::string& filename);
	void readBinaryVecInt(std::ifstream& binaryFile, ParserData* parserData);
	void readBinaryVecVec3(std::ifstream& binaryFile, ParserData* parserData, int choice);
	void readBinaryVecVec2(std::ifstream& binaryFile, ParserData* parserData);
	void readBinaryString(std::ifstream& binaryFile, ParserData* parserData);
	void readBinaryVec3(std::ifstream& binaryFile, ParserData* parserData, int choice);
	void readBinaryFloat(std::ifstream& binaryFile, ParserData* parserData);

private:
	std::vector<ParserData*> m_memoryTracker;

private:
	// Used for obj indexing
	struct OBJFace {
		unsigned int index;
		unsigned int vertexIndex;
		unsigned int uvIndex;
		unsigned int normalIndex;
	};

	std::vector<OBJFace> m_faces;
};

#endif
