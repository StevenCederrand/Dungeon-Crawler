#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H
#include "State.h"
#include "Parser/Parser.h"
#include "FBX/FBXBinReader.h"
#include "Graphics/GLinit.h"
#include "Graphics/Renderer.h"
#include "Utility/Camera.h"
#include "GameObjects/GameObjectManager.h"
#include "Graphics/LightManager.h"
#include <list>
#include "Graphics/Effects.h"
#include "../Graphics/Map.h"
#include <GUI/PlayerHealthBar.h>
#include <Gui/ScreenBlood.h>
#include <EnemyProjectile/ProjectileManager.h>

class PlayState : public State {
public:
	PlayState();
	~PlayState();

	void update(float dt);
	void renderImGUI();
	void render();
private:

	void resetPlayer();
	void constructWorld();

private:
	Parser* m_parser;
	FBXBinReader* m_FBXParser;
	GLinit* m_GLinit;
	Camera* m_camera;
	Renderer* m_renderer;
	GameObjectManager* m_gameObjectManager;
	LightManager* m_lightManager;
	GameObject* m_player;
	GameObject* m_walker;
	GameObject* m_shooter;
	GameObject* m_powerUp;
	ProjectileManager* m_projectileManager;
	GameObject* m_boss;
	PlayerHealthBar* m_playerHealthBar;
	Effects* m_effects;
	Map* m_map;
	ScreenBlood* m_screenBlood;


};

#endif
