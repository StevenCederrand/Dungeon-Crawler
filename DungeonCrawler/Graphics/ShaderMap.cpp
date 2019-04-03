#include "ShaderMap.h"
#include "System/Log.h"

std::map<std::string, Shader*> ShaderMap::m_shaderMap;

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