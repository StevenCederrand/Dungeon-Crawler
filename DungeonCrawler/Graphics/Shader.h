#ifndef _SHADER_H
#define _SHADER_H
#include <string>
//include both glm?
#include <glm\glm.hpp>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

class Shader
{
public:
	Shader(std::string vertex, std::string fragment);
	Shader(std::string vertex, std::string geometry, std::string fragment);
	~Shader();
	
	void use();
	void unuse();

	void setMat3(std::string name, glm::mat3 mat);
	void setMat4(std::string name, glm::mat4 mat);
	void setVec3(std::string name, glm::vec3 vec);
	void setVec4(std::string name, glm::vec4 vec);
	void setFloat(std::string name, float num);
	void setInt(std::string name, int num);
	int getShaderID() const;
	void setName(std::string name);
	std::string getName() const;

private:

	std::string m_name;
	int m_shaderProg;
	void shaderSetup(std::string shaderName, unsigned int &shader);

};

#endif