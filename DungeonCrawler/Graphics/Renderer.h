#ifndef _RENDERER_H
#define _RENDERER_H
#include "Utility/Camera.h"
#include "GameObjects/GameObject.h"
#include "LightManager.h"
#include "Framebuffer.h"
#include <vector>
#include <map>
#include "Shader.h"
#include "Effects.h"
#include "Map.h"
#include <EnemyProjectile/ProjectileManager.h>
#include <GUI/PlayerHealthBar.h>
#include <Gui/ScreenBlood.h>

class Renderer
{
public:

	Renderer(Camera* camera, LightManager* lightManager, Effects* effects, 
		ProjectileManager* projectileManager, PlayerHealthBar* playerHealthBar,
		Map* map, ScreenBlood* screenBlood);

	~Renderer();
	void prepareGameObject(GameObject* gameObject);
	void preparePlayerLights(Player* player);
	void render();
	
	const float LIGHTFOV = 30.f;
	const float NEAR_CLIP = 0.1f;
	const float FAR_CLIP = 50.0f;

private:
	void bindMesh(Mesh* mesh, Shader* shader);
	void unbindMesh(Mesh * mesh);
	
	void shadowPass(); //Get depth buffer
	void geometryPass();
	void renderEffects();
	void renderAnimatedEffects();
	void renderProjectiles();
	void renderHealthBar();
	void lightPass();
	void renderMap();
	void renderBlood();


	bool initRenderQuad();	
	void drawQuad();
	//Configures the view matrix for the shadow mapper
	void configureShadowMapperVM();
	
private:
	Framebuffer* m_framebuffer;
	
	Spotlight* m_playerSpotLight;
	Light* m_playerLight;

	std::map<Mesh*, std::vector<GameObject*>> m_meshes;
	std::map<Mesh*, std::vector<GameObject*>>::iterator m_meshIterator;
	
	LightManager* m_lightManager;
	Camera* m_camera;
	Effects* m_effects;

	Map* m_map;
	ProjectileManager* m_projectileManager;
	PlayerHealthBar* m_playerHealthBar;
	ScreenBlood* m_screenBlood;

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