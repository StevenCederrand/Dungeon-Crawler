#ifndef _MESHMAP_H
#define _MESHMAP_H
#include <map>
#include "Mesh.h"
#include <string>

class MeshMap {

public:
	static void addMesh(const std::string& name, Mesh* mesh);
	static Mesh* getMesh(const std::string& name);
	static void cleanUp();

private:
	static std::map<std::string, Mesh*> m_meshMap;

};


#endif
