#include "MeshMap.h"
#include "System/Log.h"
std::map<std::string, Mesh*> MeshMap::m_meshMap;

void MeshMap::addMesh(const std::string & name, Mesh * mesh)
{
	m_meshMap[name] = mesh;
}

bool MeshMap::MeshExistWithName(std::string & name)
{
	if (m_meshMap.find(name) != m_meshMap.end())
	{
		LOG_WARNING("There is already a mesh called ( MESHMAP ) " + name);
		return true;
	}

	return false;
}

Mesh * MeshMap::getMesh(const std::string & name)
{
	if (m_meshMap.find(name) == m_meshMap.end())
	{
		LOG_ERROR("Could not find mesh " + name);
	}

	return m_meshMap[name];
}

void MeshMap::cleanUp()
{
	std::map<std::string, Mesh*>::iterator it;
	
	for (it = m_meshMap.begin(); it != m_meshMap.end(); it++)
	{
		delete it->second;
	}
}
