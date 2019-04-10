#include "ShaderMap.h"
#include "System/Log.h"

std::map<std::string, Shader*> ShaderMap::m_shaderMap;

Shader* ShaderMap::createShader(std::string name, std::string vertex, std::string fragment)
{
	if (m_shaderMap.find(name) != m_shaderMap.end())
	{
		LOG_WARNING("Shader already exist: " + name);
		return nullptr;
	}
	Shader* tempShader = new Shader(vertex, fragment);
	addShader(name, tempShader);
	return tempShader;
}

Shader* ShaderMap::createShader(std::string name, std::string vertex, std::string geometry, std::string fragment) {
	if (m_shaderMap.find(name) != m_shaderMap.end())
	{
		LOG_WARNING("Shader already exist: " + name);
		return nullptr;
	}
	Shader* tempShader = new Shader(vertex, geometry, fragment);
	addShader(name, tempShader);
	return tempShader;

}

void ShaderMap::addShader(std::string name, Shader * shader)
{
	m_shaderMap[name] = shader;
}

Shader * ShaderMap::getShader(std::string name)
{
	if (m_shaderMap.find(name) == m_shaderMap.end())
	{
		LOG_ERROR("COULD NOT FIND SHADER " + name);
	}

	return m_shaderMap[name];
}

void ShaderMap::cleanUp()
{
	std::map<std::string, Shader*>::iterator it;

	for (it = m_shaderMap.begin(); it != m_shaderMap.end(); it++)
	{
		delete it->second;
	}

	m_shaderMap.clear();
}

bool ShaderMap::existShaderWithName(std::string name)
{
	if (m_shaderMap.find(name) != m_shaderMap.end()) 
	{
		LOG_WARNING("Shader already exist: " + name);
		return true;
	}
	return false;
}
