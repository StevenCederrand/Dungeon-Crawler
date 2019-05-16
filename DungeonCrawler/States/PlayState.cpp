#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Input.h"
#include "../Audio/AudioEngine.h"
#include "Graphics/MeshMap.h"
#include "Graphics/ShaderMap.h"

#pragma region GameObject_Includes
#include "GameObjects/Box.h"
#include "GameObjects/Room.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemies/Walker.h"
#include "GameObjects/Enemies/Shooter.h"

#pragma endregion

#pragma region State_Includes
#include "StateManager.h"
#include "GameOverState.h"
#include "WinState.h"
#pragma endregion

#include "Utility/Randomizer.h"
#include <chrono>

PlayState::PlayState() {

	m_parser = new Parser();
	m_GLinit = new GLinit();

	#pragma region Init
	m_camera = new Camera();
	m_effects = new Effects(m_GLinit);
	m_effects->createEmitter("BloodEmitter", "BloodParticle.png", 0.50f);
	m_effects->createEmitter("WallSmokeEmitter", "WallSmoke.png", 0.50f);
	m_effects->createEmitter("ProjectileExplosionEmitter", "whitePuff00.png", 0.50f);
	m_effects->createEmitter("EnemyHoverEmitter", "WallSmoke.png", 0.30f);
	m_effects->createEmitter("GunFlareEmitter", "GunFlare.png", 0.25f);

	AudioEngine::loadSSO("Game.sso");
	#pragma endregion
	
	//change to FBX, one Parserdata for multiple objects
	//CONTINUE HERE
	FBXParserData* enemyData = m_FBXParser->binaryMeshReading("ourFileBinary.bin");
	m_GLinit->createMeshFBX("Enemy", enemyData);
	//delete enemyData;

	#pragma region Create_Objects
	ParserData* boxData = m_parser->loadFromObj("collisionboxtest.obj");
	ParserData* playerData = m_parser->loadFromObj("MainCharacterPosed.obj");
	ParserData* doorData = m_parser->loadFromObj("doorEnd.obj");
	ParserData* roomStart = m_parser->loadFromObj("roomStart.obj");
	ParserData* roomEnd = m_parser->loadFromObj("roomEnd.obj");

	ParserData* sphereData = m_parser->loadFromObj("sphere.obj");
	ParserData* powerUpData = m_parser->loadFromObj("LifePowerUp.obj");
	//ParserData* enemyData = m_parser->loadFromObj("FlyGuyConverted.obj");

	m_GLinit->createMesh("Door", doorData);
	m_GLinit->createMesh("RoomStart", roomStart);
	m_GLinit->createMesh("RoomEnd", roomEnd);

	m_GLinit->createMesh("Box", boxData);
	m_GLinit->createMesh("PlayerModel", playerData);
	m_GLinit->createMesh("Sphere", sphereData);
	m_GLinit->createMesh("PowerUp", powerUpData);
	//m_GLinit->createMesh("Enemy", enemyData);
	#pragma endregion

	//we want to setUp the world
	constructWorld();
}

PlayState::~PlayState() {
	delete m_parser;
	delete m_GLinit;
	delete m_camera;
	delete m_gameObjectManager;
	delete m_renderer;
	delete m_lightManager;
	delete m_effects;
	delete m_map;
	delete m_projectileManager;
	delete m_playerHealthBar;
	delete m_screenBlood;
}

void PlayState::update(float dt) {

	m_map->update(dt);
	m_playerHealthBar->update(dt);
	m_gameObjectManager->update(dt);
	m_projectileManager->update(dt);
	m_effects->update(dt);
	m_camera->update(dt);
	m_lightManager->update(dt);
	m_renderer->prepareGameObjects(m_gameObjectManager->getGameObjects());
	m_screenBlood->update(dt);
	Player* player = m_gameObjectManager->getPlayer();
	if (player->getHealth()<=0)
	{
		resetPlayer();
		GameOverState* gameOver = new GameOverState();
		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		glfwSetCursor(glfwGetCurrentContext(), cursor);
		m_stateManager->pushTemporaryState(gameOver);
	}

	if (m_gameObjectManager->bossDead()) {
		WinState* winState = new WinState();
		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		glfwSetCursor(glfwGetCurrentContext(), cursor);
		m_stateManager->pushTemporaryState(winState);
	}
}

void PlayState::renderImGUI()
{
	ImGui::Begin("PlayState");

	ImGui::Text("Press shift or r.mb to DASH");

	ImGui::Text("Player [ %f%s%f%s%f%s"
		, m_player->getPosition().x
		, ", "
		, m_player->getPosition().y
		, ", "
		, m_player->getPosition().z
		, " ]");

	ImGui::NewLine();
	ImGui::Text("Nr of effects: %i" , m_effects->getTotalAmountOfParticles());
	ImGui::NewLine();
	ImGui::Text("Nr of Enemy projectiles: %i", m_projectileManager->getNumberOfEnemyProjectiles());
	ImGui::End();
}

void PlayState::render() {
	m_renderer->render();
}

void PlayState::resetPlayer()
{
	delete m_gameObjectManager;
	delete m_renderer;
	delete m_lightManager;
	delete m_playerHealthBar;
	delete m_projectileManager;
	delete m_map;
	delete m_screenBlood;
	
	//we want to setUp the world
	constructWorld();
}


void PlayState::constructWorld()
{
	Mesh* playerMesh = MeshMap::getMesh("PlayerModel");
	m_player = new Player(playerMesh, PLAYER);
	m_playerHealthBar = new PlayerHealthBar(m_GLinit, dynamic_cast<Player*>(m_player));
	m_screenBlood = new ScreenBlood(m_GLinit, dynamic_cast<Player*>(m_player));

	Camera::active = m_camera;
	m_lightManager = new LightManager();
	m_projectileManager = new ProjectileManager(m_GLinit, m_effects);
	m_gameObjectManager = new GameObjectManager(m_effects, m_projectileManager);
	m_map = new Map(m_gameObjectManager);
	m_renderer = new Renderer(m_camera, m_lightManager, m_effects, m_projectileManager, m_playerHealthBar, m_map, m_screenBlood);


	Mesh* boxMesh = MeshMap::getMesh("Box");
	Mesh* powerUpMesh = MeshMap::getMesh("PowerUp");
	Mesh* roomStart = MeshMap::getMesh("RoomStart");
	Mesh* roomEnd = MeshMap::getMesh("RoomEnd");
	Mesh* door = MeshMap::getMesh("Door");

	Room* r_roomStart = new Room(roomStart, ROOM, m_player);
	Room* r_roomEnd = new Room(roomEnd, ROOM_BOSS, m_player);

	m_gameObjectManager->addGameObject(r_roomStart);
	m_gameObjectManager->addGameObject(r_roomEnd);
	m_gameObjectManager->addGameObject(new Box(door, DOOR));
	m_gameObjectManager->addGameObject(m_player);
	
	m_projectileManager->setPlayer(m_player);

	m_powerUp = new PowerUps(powerUpMesh, POWERUPS, 5, 0, 0, false, glm::vec3(4.f, 0.5f, -2.f));
	m_gameObjectManager->addGameObject(m_powerUp);
	//m_powerUp = new PowerUps(powerUpMesh, POWERUPS, 0, 10, 0, false, glm::vec3(2.f, 0.5f, -10.f));
	//m_gameObjectManager->addGameObject(m_powerUp);
	m_powerUp = new PowerUps(powerUpMesh, POWERUPS, 0, 0, 5, true, glm::vec3(-5.f, 0.5f, -7.f));
	m_gameObjectManager->addGameObject(m_powerUp);

	m_lightManager->setSun(ShaderMap::getShader("LightPass"), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f));
	for (int i = 0; i < 2; i++)
	{
	m_lightManager->addLight(
		// Position
		glm::vec3(
			Randomizer::single(-20.f, 20.f),
			40.f,
			Randomizer::single(-60.f, 20.f)),
		// Color
		glm::vec3(
			Randomizer::single(0.f, 255.f) / 255.f,
			Randomizer::single(0.f, 255.f) / 255.f,
			Randomizer::single(0.f, 255.f) / 255.f),
		60.f, m_gameObjectManager);

	}

	//Used for the player flashlight & shadow mapping from the 
	//flashlights view
	m_renderer->preparePlayerLights(m_gameObjectManager->getPlayer());
}
