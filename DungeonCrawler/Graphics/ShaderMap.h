#ifndef _SHADERMAP_H
#define _SHADERMAP_H
#include "Shader.h"
#include <map>
#include <string>

class ShaderMap
{
public:
	static void createShader(std::string name, std::string vertex,
		std::string fragment);

	static void createShader(std::string name, std::string vertex,
		std::string geometry, std::string fragment);

	static void addShader(std::string name, Shader* shader);
	static Shader* getShader(std::string name);
	static void cleanUp();
	static bool existShaderWithName(std::string name);


private:

	static std::map<std::string, Shader*> m_shaderMap;
};
#endif // !_SHADERMAP_H