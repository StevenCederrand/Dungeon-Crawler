#ifndef _RENDERER_H
#define _RENDERER_H
#include "Utility/Camera.h"
#include "GameObjects/GameObject.h"
#include "Framebuffer.h"
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
	
	void forwardPass();
	void geometryPass();
	void lightPass();

	bool initRenderQuad();	

private:
	Framebuffer m_framebuffer;

	
	std::map<Mesh*, std::vector<GameObject*>> m_meshes;
	std::map<Mesh*, std::vector<GameObject*>>::iterator m_meshIterator;
	Camera* m_camera;


	unsigned int rQuadVAO;
	unsigned int rQuadVBO;
	float rQuadData[24] = {
		//VP			UV
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
};

#endif