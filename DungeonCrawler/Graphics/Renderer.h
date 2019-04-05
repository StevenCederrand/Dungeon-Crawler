#ifndef _RENDERER_H
#define _RENDERER_H
#include "Utility/Camera.h"
#include "GameObjects/GameObject.h"
#include "LightManager.h"
#include "Framebuffer.h"
#include <vector>
#include <map>
#include "Shader.h"

class Renderer
{
public:
	Renderer(Camera* camera, LightManager* lightManager);
	~Renderer();

	void prepareGameObjects(const std::vector<GameObject*>& gameObjects);
	void render();

private:
	void bindMesh(Mesh* mesh, Shader* shader);
	void unbindMesh();
	
	void forwardPass();
	void geometryPass();
	void lightPass();

	bool initRenderQuad();	
	void drawQuad();
private:
	Framebuffer* m_framebuffer;

	std::map<Mesh*, std::vector<GameObject*>> m_meshes;
	std::map<Mesh*, std::vector<GameObject*>>::iterator m_meshIterator;
	
	LightManager* m_lightManager;
	Camera* m_camera;


	unsigned int m_rQuadVAO;
	unsigned int m_rQuadVBO;
	float m_rQuadData[24] = {
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