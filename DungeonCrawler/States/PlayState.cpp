#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Log.h"
#include "System/Input.h"
#include "StateManager.h"
#include "../Audio/AudioEngine.h"
#include "Graphics/MeshMap.h"
#include "Graphics/ShaderMap.h"
#include "GameObjects/Box.h"
#include "GameObjects/Room.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemies/Walker.h"
#include "GameObjects/Enemies/Shooter.h"
#include "GameOverState.h"

#include "Utility/Randomizer.h"
#include <chrono>
#include <thread>

void loadObj(Parser* parser, GLinit* glInit, std::string filename, std::string name) {
	ParserData* data = parser->loadFromObj(filename);
	glInit->createMesh(name, data);
}

PlayState::PlayState() {

	m_parser = new Parser();
	m_GLinit = new GLinit();

	#pragma region Init
	m_camera = new Camera();
	m_effects = new Effects(m_GLinit);
	m_effects->createEmitter("BloodEmitter", "BloodParticle.png", 0.5f);
	m_effects->createEmitter("WallSmokeEmitter", "WallSmoke.png", 0.5f);
	m_effects->createEmitter("GunFlareEmitter", "GunFlare.png", 0.25f);

	Camera::active = m_camera;
	m_lightManager = new LightManager()	;
	m_renderer = new Renderer(m_camera, m_lightManager, m_effects);
	m_gameObjectManager = new GameObjectManager(m_effects);
	AudioEngine::loadSSO("Game.sso");
	#pragma endregion
		
	#pragma region Create_Objects
	ParserData* boxData = m_parser->loadFromObj("collisionboxtest.obj");
	ParserData* playerData = m_parser->loadFromObj("MainCharacterPosed.obj");
	ParserData* doorData = m_parser->loadFromObj("doorEnd.obj");
	ParserData* roomStart = m_parser->loadFromObj("roomStart.obj");
	ParserData* roomEnd = m_parser->loadFromObj("roomEnd.obj");


	ParserData* sphereData = m_parser->loadFromObj("sphere.obj");
	ParserData* powerUpData = m_parser->loadFromObj("LifePowerUp.obj");
	ParserData* enemyData = m_parser->loadFromObj("FlyGuyConverted.obj");

	m_GLinit->createMesh("Door", doorData);
	m_GLinit->createMesh("RoomStart", roomStart);
	m_GLinit->createMesh("RoomEnd", roomEnd);


	m_GLinit->createMesh("Box", boxData);
	m_GLinit->createMesh("PlayerModel", playerData);
	m_GLinit->createMesh("Sphere", sphereData);
	m_GLinit->createMesh("PowerUp", powerUpData);
	m_GLinit->createMesh("Enemy", enemyData);
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
}

void PlayState::update(float dt) {

	m_gameObjectManager->update(dt);
	m_effects->update(dt);
	m_camera->update(dt);
	m_lightManager->update(dt);
	
	m_renderer->prepareGameObjects(m_gameObjectManager->getGameObjects());


	Player* player = m_gameObjectManager->getPlayer();
	if (player->getHealth()<=0)
	{
		resetPlayer();
		GameOverState* gameOver = new GameOverState();
		m_stateManager->pushTemporaryState(gameOver);
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
	ImGui::Text("Nr of lasers: %i" , m_effects->getTotalAmountOfParticles());

	ImGui::End();
}

void PlayState::render() {
	m_renderer->render();
}
//A general function that creates is targeted to room creation
void PlayState::createRoom(std::string filename, std::string key) {
	ParserData* roomData = m_parser->loadFromObj(filename);
	Mesh* room = m_GLinit->createMesh(key, roomData);
	//m_gameObjectManager->addGameObject(new Room(room, ROOM, glm::vec3(0.f, 0.f, 0.f)));
}

void PlayState::spawnEnemies(int minX, int maxX, int minZ, int maxZ) {
	Mesh* boxMesh = MeshMap::getMesh("Character");
	Mesh* cubeMesh = MeshMap::getMesh("Cube");
	//check the collsiion and then write to binary
	bool enemy = false;
	for (int i = 0; i < 20; i++) {
		if (!enemy) {
			enemy = true;
			m_gameObjectManager->addGameObject(new Box(boxMesh, BOX,
				glm::vec3(
					Randomizer::single(minX, maxX), //-30.f, 30.f
					0.f,
					Randomizer::single(minZ, maxZ) //-15.f, 15.f
				)));
		}
		else {
			m_gameObjectManager->addGameObject(new Box(cubeMesh, BOX,
				glm::vec3(
					Randomizer::single(minX, maxX),
					0.f,
					Randomizer::single(minZ, maxZ) //-15.f, 15.f
				)));
		}
	}

	//m_shooter = new Shooter(boxMesh, SHOOTER);
	//m_gameObjectManager->addGameObject(m_shooter);
	//m_walker = new Walker(boxMesh, WALKER);
	m_gameObjectManager->addGameObject(m_walker);
}

void PlayState::resetPlayer()
{
	delete m_gameObjectManager;
	delete m_renderer;
	delete m_lightManager;

	m_lightManager = new LightManager();
	m_renderer = new Renderer(m_camera, m_lightManager, m_effects);
	m_gameObjectManager = new GameObjectManager(m_effects);
	//we want to setUp the world
	constructWorld();
}


void PlayState::constructWorld()
{

	Mesh* boxMesh = MeshMap::getMesh("Box");
	Mesh* playerMesh = MeshMap::getMesh("PlayerModel");

	Mesh* powerUpMesh = MeshMap::getMesh("PowerUp");
	Mesh* enemyMesh = MeshMap::getMesh("Enemy");
	Mesh* roomStart = MeshMap::getMesh("RoomStart");
	Mesh* roomEnd = MeshMap::getMesh("RoomEnd");
	Mesh* door = MeshMap::getMesh("Door");

	m_lightManager->setSun(ShaderMap::getShader("LightPass"), glm::vec3(-5.f, 1.5f, 0.f), glm::vec3(0.8f, .8f, 0.8f));
	m_lightManager->addLight(glm::vec3(5.f), glm::vec3(0.5f, 0.f, 1.f), 10.f, m_gameObjectManager);
	m_lightManager->addLight(glm::vec3(0.f, 5.f, -5.f), glm::vec3(0.0f, 1.f, 0.f), 10.f, m_gameObjectManager);


	//Room * room = new Room(roomMesh, ROOM, m_player, glm::vec3(0.f, 0.f, 0.f));
	Room* r_roomStart = new Room(roomStart, ROOM, m_player);
	Room* r_roomEnd = new Room(roomEnd, ROOM, m_player);

	m_gameObjectManager->addGameObject(r_roomStart);
	m_gameObjectManager->addGameObject(r_roomEnd);
	m_gameObjectManager->addGameObject(new Box(door, DOOR));

	m_player = new Player(playerMesh, PLAYER);
	m_gameObjectManager->addGameObject(m_player);



	m_powerUp = new PowerUps(powerUpMesh, POWERUPS, 10, 0, 0, false, glm::vec3(10.f, 0.f, -5.f));
	m_gameObjectManager->addGameObject(m_powerUp);
	m_powerUp = new PowerUps(powerUpMesh, POWERUPS, 0, 10, 0, false, glm::vec3(2.f, 0.f, -10.f));
	m_gameObjectManager->addGameObject(m_powerUp);
	m_powerUp = new PowerUps(powerUpMesh, POWERUPS, 0, 0, 10, true, glm::vec3(-5.f, 0.f, -7.f));
	m_gameObjectManager->addGameObject(m_powerUp);



	for (int i = 0; i < 5; i++)
	{
	m_lightManager->addLight(
		// Position
		glm::vec3(
			Randomizer::single(-20.f, 20.f),
			5.f,
			Randomizer::single(-20.f, 20.f)),
		// Color
		glm::vec3(
			Randomizer::single(0.f, 255.f) / 255.f,
			Randomizer::single(0.f, 255.f) / 255.f,
			Randomizer::single(0.f, 255.f) / 255.f),
		25.f, m_gameObjectManager);

	}

	//m_shooter = new Shooter(enemyMesh, SHOOTER);
	//m_gameObjectManager->addGameObject(m_shooter);
	m_boss = new Boss(enemyMesh, BOSS, r_roomStart, glm::vec3(2.f, 0.f, 9.f));
	m_gameObjectManager->addGameObject(m_boss);
	for (int i = 0; i < 5; i++)
	{
	m_walker = new Walker(enemyMesh, WALKER, r_roomStart, glm::vec3(
		Randomizer::single(-10.0f, 10.0f),
		0.f,
		Randomizer::single(-10.0f, 10.0f)));
	m_gameObjectManager->addGameObject(m_walker);
	}

	//Used for the player flashlight & shadow mapping from the 
	//flashlights view
	m_renderer->preparePlayerLights(m_gameObjectManager->getPlayer());
}
