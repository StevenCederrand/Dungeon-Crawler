#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include "State.h"
#include "Parser/Parser.h"
#include "Graphics/GLinit.h"
#include "Graphics/Renderer.h"
#include "Utility/Camera.h"
#include "GameObjects/GameObjectManager.h"
#include "Graphics/LightManager.h"
#include <list>
#include "Graphics/Effects.h"

class PlayState : public State {
public:
	PlayState();
	~PlayState();

	void update(float dt);
	void renderImGUI();
	void render();

private:
	void createRoom(std::string filename, std::string key);
	void spawnEnemies(int minX, int maxX, int minZ, int maxZ);

private:
	Parser* m_parser;
	GLinit* m_GLinit;
	Camera* m_camera;
	Renderer* m_renderer;
	GameObjectManager* m_gameObjectManager;
	LightManager* m_lightManager;
	GameObject* m_player;
	GameObject* m_walker;
	GameObject* m_shooter;

	Effects* m_effects;
};

#endif
