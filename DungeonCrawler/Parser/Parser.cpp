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

	std::vector<std::string> filenameString = split(filename, '.');
	std::ifstream  binaryExist(Binaries + filenameString[0]);
	ParserData* data = new ParserData(CAPACITY);
	if (binaryExist.good())
	{
		//ParserData* data = loadFromBinary(filenameString[0]);
		loadFromBinary(data, filenameString[0]);
		m_memoryTracker.emplace_back(data);
		return data;
	}
	
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


	std::string line;
	std::string MTLfile = "";
	GLuint indexCount = 0;
	bool isParsingCollider = false;

	glm::vec3 min = glm::vec3(100000);
	glm::vec3 max = glm::vec3(-100000);


	while (std::getline(objFile, line))
	{

		std::vector<std::string> attribs = split(line, ' ');

		if (attribs.size() == 0)
			continue;

		this->stringClean(attribs);

		if (attribs[0] == "#") {

			if (attribs.size() < 3)
				continue;

			if (attribs[1] == "object" && attribs[2] == "collision")
			{

				isParsingCollider = true;
			}
			continue;
		}

		

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
				if (min.z > z) min.z = z;

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

	data->setBoundingBox(min, max);

	writeToBinary(data, filenameString[0]);
	
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
//obj files to binary files in binary map
void Parser::writeToBinary(ParserData* data, const std::string& filename)
{
	std::ofstream binaryFile(Binaries + filename, std::ios::binary);

	std::vector<GLuint> indices = data->getIndices();
	writeBinaryVecInt(binaryFile, indices);
	
	std::vector<glm::vec3> vertices = data->getVertices();
	writeBinaryVecVec3(binaryFile, vertices);

	std::vector<glm::vec2> uvs = data->getUvs();
	writeBinaryVecVec2(binaryFile, uvs);

	std::vector<glm::vec3> normals = data->getNormals();
	writeBinaryVecVec3(binaryFile, normals);

	std::string textureFilename = data->getTextureFilename();
	writeBinaryString(binaryFile, textureFilename);

	glm::vec3 diffuseColor = data->getDiffuseColor();
	writeBinaryVec3(binaryFile, diffuseColor);

	glm::vec3 specularColor = data->getSpecularColor();
	writeBinaryVec3(binaryFile, specularColor);

	glm::vec3 ambientColor = data->getAmbientColor();
	writeBinaryVec3(binaryFile, ambientColor);

	GLfloat shininess = data->getShininess();
	writeBinaryFloat(binaryFile, shininess);

	binaryFile.close();
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


void Parser::writeBinaryVecInt(std::ofstream& binaryFile, std::vector<GLuint> vector)
{
	//LOG_ERROR("writeBinaryVecInt ");
	//std::ofstream binaryFile(Binaries + filename, std::ios::binary);
	if (!binaryFile.is_open())
	{
		//LOG_ERROR("Binary file not found " + filename);
		return;
	}
	//create string to have the data in
	std::string stringvector;

	//loops to fill the string with the data
	for (int i = 0; i < vector.size(); i++)
	{
		stringvector.append(std::to_string(vector[i]));
		stringvector.append(" ");
	}

	//size of the string
	int indicesStringSize = stringvector.size();

	//make a string of the size ( to write it to the file)
	std::string indicesStringInt = std::to_string(indicesStringSize);
	for (int i = indicesStringInt.size(); i < 10; i++)
	{
		indicesStringInt.append(" ");
	}

	//write to the file
	binaryFile.write(indicesStringInt.c_str(), indicesStringInt.size());
	binaryFile.write(stringvector.c_str(), indicesStringSize);

	//binaryFile.close();
}

void Parser::writeBinaryVecVec3(std::ofstream& binaryFile, std::vector<glm::vec3> vector)
{
	
	if (!binaryFile.is_open())
	{
		
		return;
	}
	//create string to have the data in
	std::string stringvector;

	//loops to fill the string with the data
	for (int i = 0; i < vector.size(); i++)
	{
		stringvector.append(std::to_string(vector[i].x));
		stringvector.append(" ");
		stringvector.append(std::to_string(vector[i].y));
		stringvector.append(" ");
		stringvector.append(std::to_string(vector[i].z));
		stringvector.append(" ");
	}

	//size of the string
	int indicesStringSize = stringvector.size();

	//make a string of the size ( to write it to the file)
	std::string indicesStringInt = std::to_string(indicesStringSize);
	for (int i = indicesStringInt.size(); i < 10; i++)
	{
		indicesStringInt.append(" ");
	}

	//write to the file
	binaryFile.write(indicesStringInt.c_str(), indicesStringInt.size());
	binaryFile.write(stringvector.c_str(), indicesStringSize);

	//binaryFile.close();
}

void Parser::writeBinaryVecVec2(std::ofstream& binaryFile, std::vector<glm::vec2> vector)
{
	//std::ofstream binaryFile(Binaries + filename, std::ios::binary);
	if (!binaryFile.is_open())
	{
		//LOG_ERROR("Binary file not found " + filename);
		return;
	}
	//create string to have the data in
	std::string stringvector;

	//loops to fill the string with the data
	for (int i = 0; i < vector.size(); i++)
	{
		stringvector.append(std::to_string(vector[i].x));
		stringvector.append(" ");
		stringvector.append(std::to_string(vector[i].y));
		stringvector.append(" ");

	}

	//size of the string
	int indicesStringSize = stringvector.size();

	//make a string of the size ( to write it to the file)
	std::string indicesStringInt = std::to_string(indicesStringSize);
	for (int i = indicesStringInt.size(); i < 10; i++)
	{
		indicesStringInt.append(" ");
	}

	//write to the file
	binaryFile.write(indicesStringInt.c_str(), indicesStringInt.size());
	binaryFile.write(stringvector.c_str(), indicesStringSize);

	//binaryFile.close();
}

void Parser::writeBinaryString(std::ofstream& binaryFile, std::string string)
{
	//std::ofstream binaryFile(Binaries + filename, std::ios::binary);
	if (!binaryFile.is_open())
	{
	//	LOG_ERROR("Binary file not found " + filename);
		return;
	}

	//size of the string
	int indicesStringSize = string.size();

	//make a string of the size ( to write it to the file)
	std::string indicesStringInt = std::to_string(indicesStringSize);
	for (int i = indicesStringInt.size(); i < 10; i++)
	{
		indicesStringInt.append(" ");
	}

	//write to the file
	binaryFile.write(indicesStringInt.c_str(), indicesStringInt.size());
	binaryFile.write(string.c_str(), indicesStringSize);

	//binaryFile.close();

}

void Parser::writeBinaryVec3(std::ofstream& binaryFile, glm::vec3 vector)
{
	//std::ofstream binaryFile(Binaries + filename, std::ios::binary);
	if (!binaryFile.is_open())
	{
		//LOG_ERROR("Binary file not found " + filename);
		return;
	}
	//create string to have the data in
	std::string stringvector;

	
	//the string with the data
	stringvector.append(std::to_string(vector.x));
	stringvector.append(" ");
	stringvector.append(std::to_string(vector.y));
	stringvector.append(" ");
	stringvector.append(std::to_string(vector.z));
	stringvector.append(" ");
	

	//size of the string
	int indicesStringSize = stringvector.size();

	//make a string of the size ( to write it to the file)
	std::string indicesStringInt = std::to_string(indicesStringSize);
	for (int i = indicesStringInt.size(); i < 10; i++)
	{
		indicesStringInt.append(" ");
	}

	//write to the file
	binaryFile.write(indicesStringInt.c_str(), indicesStringInt.size());
	binaryFile.write(stringvector.c_str(), indicesStringSize);

	//binaryFile.close();
}

void Parser::writeBinaryFloat(std::ofstream& binaryFile, GLfloat floatValue)
{

	//std::ofstream binaryFile(Binaries + filename, std::ios::binary);
	if (!binaryFile.is_open())
	{
		//LOG_ERROR("Binary file not found " + filename);
		return;
	}
	//create string to have the data in
	std::string stringvector;


	//the string with the data
	stringvector.append(std::to_string(floatValue));
	stringvector.append(" ");


	//size of the string
	int indicesStringSize = stringvector.size();

	//make a string of the size ( to write it to the file)
	std::string indicesStringInt = std::to_string(indicesStringSize);
	for (int i = indicesStringInt.size(); i < 10; i++)
	{
		indicesStringInt.append(" ");
	}

	//write to the file
	binaryFile.write(indicesStringInt.c_str(), indicesStringInt.size());
	binaryFile.write(stringvector.c_str(), indicesStringSize);

	//binaryFile.close();

}

void Parser::loadFromBinary(ParserData* data, const std::string & filename)
{
	//ParserData* data = new ParserData(CAPACITY);

	std::ifstream binaryFile(Binaries + filename, std::ios::binary);
	readBinaryVecInt(binaryFile, data);

	readBinaryVecVec3(binaryFile, data, 0);
	readBinaryVecVec2(binaryFile, data);
	readBinaryVecVec3(binaryFile, data, 1);

	readBinaryString(binaryFile, data);

	readBinaryVec3(binaryFile, data, 0);
	readBinaryVec3(binaryFile, data, 1);
	readBinaryVec3(binaryFile, data, 2);

	binaryFile.close();
}

void Parser::readBinaryVecInt(std::ifstream & binaryFile, ParserData* parserData)
{
	//read the value first (how big the other read should be)
	char* textInt = new char[10];
	binaryFile.read(textInt, 10);

	//convert it  from char* to int
	char *tempa;
	int readSize = strtol(textInt, &tempa, 10);
	delete[] textInt;

	//make a char pointer to read to, readsize is the size
	char* text = new char[readSize + 1];
	binaryFile.read(text, readSize);
	
	//make a vector string and fill it with the split function
	text[readSize] = '\0';
	std::string stringText= text;
	delete[] text;
	std::vector<std::string> vecString = split(stringText, ' ');

	
	//fill the parserData with the Information
	for (int i = 0; i < vecString.size(); i++)
	{
		GLuint tempGL = std::stoi(vecString[i]);
		parserData->addIndex(tempGL);

	}
	vecString.clear();
}
//vertex==0, normal==1
void Parser::readBinaryVecVec3(std::ifstream & binaryFile, ParserData * parserData, int choice)
{
	//read the value first (how big the other read should be)
	char* textInt = new char[10];
	binaryFile.read(textInt, 10);

	//convert it  from char* to int
	char *tempa;
	int readSize = strtol(textInt, &tempa, 10);
	delete[] textInt;

	//make a char pointer to read to, readsize is the size
	char* text = new char[readSize + 1];
	binaryFile.read(text, readSize);

	//make a vector string and fill it with the split function
	text[readSize] = '\0';
	std::string stringText = text;
	delete[] text;
	std::vector<std::string> vecString = split(stringText, ' ');

	//fill the parserData with the Information
	for (int i = 0; (i+2) < vecString.size(); i)
	{
		if (vecString[i] != "") 
		{
			glm::vec3 tempGL;
			tempGL.x = std::stof(vecString[i], NULL);
			tempGL.y = std::stof(vecString[i+1], NULL);
			tempGL.z = std::stof(vecString[i+2], NULL);
			if (choice == 0) 
			{
				parserData->addVertex(tempGL);
			}
			else if (choice == 1)
			{
				parserData->addNormal(tempGL);
			}
			i += 3;
		}
		else
		{
			i++;
		}
	}
	vecString.clear();
}

void Parser::readBinaryVecVec2(std::ifstream & binaryFile, ParserData * parserData)
{
	//read the value first (how big the other read should be)
	char* textInt = new char[10];
	binaryFile.read(textInt, 10);

	//convert it  from char* to int
	char *tempa;
	int readSize = strtol(textInt, &tempa, 10);
	delete[] textInt;

	//make a char pointer to read to, readsize is the size
	char* text = new char[readSize + 1];
	binaryFile.read(text, readSize);

	//make a vector string and fill it with the split function
	text[readSize] = '\0';
	std::string stringText = text;
	delete[] text;
	std::vector<std::string> vecString = split(stringText, ' ');

	//fill the parserData with the Information
	for (int i = 0; (i+1) < vecString.size(); i)
	{
		glm::vec2 tempGL;
		tempGL.x = std::stof(vecString[i], NULL);
		tempGL.y = std::stof(vecString[i + 1], NULL);


		parserData->addUV(tempGL);
		i += 2;
	}
	vecString.clear();
}

void Parser::readBinaryString(std::ifstream & binaryFile, ParserData * parserData)
{
	//read the value first (how big the other read should be)
	char* textInt = new char[10];
	binaryFile.read(textInt, 10);

	//convert it  from char* to int
	char *tempa;
	int readSize = strtol(textInt, &tempa, 10);
	delete[] textInt;

	//make a char pointer to read to, readsize is the size
	char* text = new char[readSize + 1];
	binaryFile.read(text, readSize);

	//make a vector string and fill it with the split function
	text[readSize] = '\0';
	std::string stringText = text;
	delete[] text;
	
	//write the texture name to the parserData
	parserData->setTextureFilename(stringText);
}
//diffuse==0, specular==1 and ambient==2
void Parser::readBinaryVec3(std::ifstream & binaryFile, ParserData * parserData, int choice)
{
	//read the value first (how big the other read should be)
	char* textInt = new char[10];
	binaryFile.read(textInt, 10);

	//convert it  from char* to int
	char *tempa;
	int readSize = strtol(textInt, &tempa, 10);
	delete[] textInt;

	//make a char pointer to read to, readsize is the size
	char* text = new char[readSize + 1];
	binaryFile.read(text, readSize);

	//make a vector string and fill it with the split function
	text[readSize] = '\0';
	std::string stringText = text;
	delete[] text;
	std::vector<std::string> vecString = split(stringText, ' ');

	//fill the parserData with the Information
	
	glm::vec3 tempGL;
	tempGL.x = std::stof(vecString[0], NULL);
	tempGL.y = std::stof(vecString[1], NULL);
	tempGL.y = std::stof(vecString[2], NULL);

	if (choice == 0) 
	{

	parserData->setDiffuseColor(tempGL.x, tempGL.y, tempGL.z);
	}
	else if (choice == 1) 
	{
		parserData->setSpecularColor(tempGL.x, tempGL.y, tempGL.z);
	}
	else if (choice == 2)
	{
		parserData->setAmbientColor(tempGL.x, tempGL.y, tempGL.z);
	}
	vecString.clear();
}

void Parser::readBinaryFloat(std::ifstream & binaryFile, ParserData * parserData)
{
	//read the value first (how big the other read should be)
	char* textInt = new char[10];
	binaryFile.read(textInt, 10);

	//convert it  from char* to int
	char *tempa;
	int readSize = strtol(textInt, &tempa, 10);
	delete[] textInt;

	//make a char pointer to read to, readsize is the size
	char* text = new char[readSize + 1];
	binaryFile.read(text, readSize);

	//make a vector string and fill it with the split function
	text[readSize] = '\0';
	std::string stringText = text;
	delete[] text;
	//std::vector<std::string> vecString = split(stringText, ' ');
	GLuint tempGL = std::stof(stringText);
	//fill the parserData with the Information
	parserData->setShininess(tempGL);
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

