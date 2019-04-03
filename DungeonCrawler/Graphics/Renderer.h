#ifndef _RENDERER_H
#define _RENDERER_H
#include "Utility/Camera.h"
#include "GameObjects/GameObject.h"
#include <vector>
#include <map>


class Renderer
{
public:
	Renderer(Camera* camera);
	~Renderer();
	void prepareGameObjects(const std::vector<GameObject*>& gameObjects);
	void render();

private:
	void bindMesh(Mesh* mesh);
	void unbindMesh();

private:
	std::map<Mesh*, std::vector<GameObject*>> m_meshes;
	std::map<Mesh*, std::vector<GameObject*>>::iterator m_meshIterator;
	Camera* m_camera;

};

#endif