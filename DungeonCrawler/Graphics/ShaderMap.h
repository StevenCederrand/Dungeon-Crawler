#ifndef _SHADERMAP_H
#define _SHADERMAP_H
#include "Shader.h"
#include <map>
#include <string>

class ShaderMap
{
public:

	static void addShader(std::string name, Shader* shader);
	static Shader* getShader(std::string name);
	static void cleanUp();


private:

	static std::map<std::string, Shader*> m_shaderMap;
};
#endif // !_SHADERMAP_H