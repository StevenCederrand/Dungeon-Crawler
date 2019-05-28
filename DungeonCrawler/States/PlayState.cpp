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
	m_FBXParser = new FBXParser();
	m_GLinit = new GLinit();

	#pragma region Init
	m_camera = new Camera();
	m_effects = new Effects(m_GLinit);
	m_effects->createEmitter("BloodEmitter", "BloodParticle.png", 0.50f);
	m_effects->createEmitter("WallSmokeEmitter", "WallSmoke.png", 0.50f);
	m_effects->createEmitter("ProjectileExplosionEmitter", "whitePuff00.png", 0.50f);
	m_effects->createEmitter("EnemyHoverEmitter", "WallSmoke.png", 0.30f);
	m_effects->createEmitter("EnemySpawnEmitter", "WallSmoke.png", 0.75f);
	m_effects->createEmitter("GunFlareEmitter", "GunFlare.png", 0.25f);
	m_effects->createEmitter("HealthPickupEmitter", "HealthSymbol.png", 0.5f);
	m_effects->createEmitter("SpeedPickupEmitter", "ArrowUpSymbol.png", 0.5f);
	m_effects->createEmitter("DamageBuffEmitter", "DamageBuffSymbol.png", 0.5f);
	m_effects->createAnimatedEmitter("summonCircle", "summonCircleSheet.png", 1920, 64, 30, 0.025f, false, 2.0f);
	m_effects->createAnimatedEmitter("bossSummonCircle", "BossSummonCircleSheet.png", 1920, 64, 30, 0.025f, false, 3.0f);
	AudioEngine::loadSSO("Game.sso");
	#pragma endregion
	
	//change to FBX, one Parserdata for multiple objects
	//CONTINUE HERE

	FBXParserData* enemyData = m_FBXParser->binaryMeshReading(FBXPath + "flyGuy.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("Enemy", enemyData);
	delete enemyData;


	FBXParserData* barrelData = m_FBXParser->binaryMeshReading(FBXPath + "barrel.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("barrel", barrelData);
	delete barrelData;

	FBXParserData* barrelStandingData = m_FBXParser->binaryMeshReading(FBXPath + "barrelStanding.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("barrelStanding", barrelStandingData);
	delete barrelStandingData;

	FBXParserData* barsData = m_FBXParser->binaryMeshReading(FBXPath + "bars.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("bars", barsData);
	delete barsData;

	FBXParserData* benchData = m_FBXParser->binaryMeshReading(FBXPath + "bench.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("bench", benchData);
	delete benchData;

	FBXParserData* chairData = m_FBXParser->binaryMeshReading(FBXPath + "chair.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("chair", chairData);
	delete chairData;
	
	FBXParserData* closedCellData = m_FBXParser->binaryMeshReading(FBXPath + "closedCell.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("closedCell", closedCellData);
	delete closedCellData;

	FBXParserData* cookerData = m_FBXParser->binaryMeshReading(FBXPath + "cooker.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("cooker", cookerData);
	delete cookerData;

	FBXParserData* cratesData = m_FBXParser->binaryMeshReading(FBXPath + "crates.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("crates", cratesData);
	delete cratesData;

	FBXParserData* heatersData = m_FBXParser->binaryMeshReading(FBXPath + "heaters.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("heaters", heatersData);
	delete heatersData;

	FBXParserData* shelfData = m_FBXParser->binaryMeshReading(FBXPath + "shelf.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("shelf", shelfData);
	delete shelfData;

	FBXParserData* tableData = m_FBXParser->binaryMeshReading(FBXPath + "table.bin"); //SHOULD MAKE BINARY
	m_GLinit->createMeshFBX("table", tableData);
	delete tableData;



	#pragma region Create_Objects
	ParserData* boxData = m_parser->loadFromObj("collisionboxtest.obj");
	ParserData* playerData = m_parser->loadFromObj("MainCharacterPosed.obj");
	

	//ParserData* roomStart = m_parser->loadFromObj("roomStart.obj");

	//ParserData* roomEnd = m_parser->loadFromObj("roomEnd.obj");

	ParserData* sphereData = m_parser->loadFromObj("sphere.obj");
	ParserData* powerUpData = m_parser->loadFromObj("LifePowerUp.obj");
	//ParserData* enemyData = m_parser->loadFromObj("FlyGuyConverted.obj");


	//m_GLinit->createMesh("RoomStart", roomStart);
	//m_GLinit->createMesh("RoomEnd", roomEnd);

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

	//Fbx
	delete m_FBXParser;
}

void PlayState::update(float dt) {

	m_map->update(dt);
	m_playerHealthBar->update(dt);
	m_gameObjectManager->update(dt);
	m_projectileManager->update(dt);
	m_effects->update(dt);
	m_camera->update(dt);
	m_lightManager->update(dt);
	m_lightManager->setPlayerLightPosition(glm::vec3(m_player->getPosition().x, 15.0f, m_player->getPosition().z));
	m_lightManager->setSunPosition(glm::vec3(m_player->getPosition().x, 20.0f, m_player->getPosition().z));
	//m_renderer->prepareGameObjects(m_gameObjectManager->getGameObjects());
	m_screenBlood->update(dt);
	Player* player = m_gameObjectManager->getPlayer();
	if (player->getHealth() <= 0 || Input::isKeyPressed(GLFW_KEY_9))
	{
		resetPlayer();
		AudioEngine::unloadSSO("Game.sso");
		GameOverState* gameOver = new GameOverState();
		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		glfwSetCursor(glfwGetCurrentContext(), cursor);
		m_stateManager->pushTemporaryState(gameOver);
	}

	if (m_gameObjectManager->gameFinished() || Input::isKeyPressed(GLFW_KEY_8)) {
		WinState* winState = new WinState();
		AudioEngine::unloadSSO("Game.sso");
		GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		glfwSetCursor(glfwGetCurrentContext(), cursor);
		m_stateManager->pushTemporaryState(winState);
	}

	m_gameObjectManager->gameFinished();
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
	m_projectileManager = new ProjectileManager(m_GLinit, m_effects, "blackProjectileSheet.png", 768, 64, 12, 0.025f);
	m_gameObjectManager = new GameObjectManager(m_effects, m_projectileManager);
	m_map = new Map(m_gameObjectManager);
	m_renderer = new Renderer(m_camera, m_lightManager, m_effects, m_projectileManager, m_playerHealthBar, m_map, m_screenBlood);
	m_gameObjectManager->setRendererRef(m_renderer);

	Mesh* boxMesh = MeshMap::getMesh("Box");
	Mesh* powerUpMesh = MeshMap::getMesh("PowerUp");

	
	Mesh* barrelMesh = MeshMap::getMesh("barrel");
	GameObject* barrelObj = new Box(barrelMesh, Type::BOX);
	m_gameObjectManager->addGameObject(barrelObj);

	Mesh* barrelStandingMesh = MeshMap::getMesh("barrelStanding");
	GameObject* barrelStandingObj = new Box(barrelStandingMesh, Type::BOX);
	m_gameObjectManager->addGameObject(barrelStandingObj);

	Mesh* barsMesh = MeshMap::getMesh("bars");
	GameObject* barsObj = new Box(barsMesh, Type::BOX);
	m_gameObjectManager->addGameObject(barsObj);

	Mesh* benchMesh = MeshMap::getMesh("bench");
	GameObject* benchObj = new Box(benchMesh, Type::BOX);
	m_gameObjectManager->addGameObject(benchObj);

	Mesh* chairMesh = MeshMap::getMesh("chair");
	GameObject* chairObj = new Box(chairMesh, Type::BOX);
	m_gameObjectManager->addGameObject(chairObj);

	Mesh* closedCellMesh = MeshMap::getMesh("closedCell");
	GameObject* closedCellObj = new Box(closedCellMesh, Type::BOX);
	m_gameObjectManager->addGameObject(closedCellObj);

	Mesh* cookerMesh = MeshMap::getMesh("cooker");
	GameObject* cookerObj = new Box(cookerMesh, Type::BOX);
	m_gameObjectManager->addGameObject(cookerObj);

	Mesh* cratesMesh = MeshMap::getMesh("crates");
	GameObject* cratesObj = new Box(cratesMesh, Type::BOX);
	m_gameObjectManager->addGameObject(cratesObj);

	Mesh* heatersMesh = MeshMap::getMesh("heaters");
	GameObject* heatersObj = new Box(heatersMesh, Type::BOX);
	m_gameObjectManager->addGameObject(heatersObj);

	Mesh* shelfMesh = MeshMap::getMesh("shelf");
	GameObject* shelfObj = new Box(shelfMesh, Type::BOX);
	m_gameObjectManager->addGameObject(shelfObj);

	Mesh* tableMesh = MeshMap::getMesh("table");
	GameObject* tableObj = new Box(tableMesh, Type::BOX);
	m_gameObjectManager->addGameObject(tableObj);


	m_gameObjectManager->addGameObject(m_player);
	
	m_projectileManager->setPlayer(m_player);
	addRoom();

	m_lightManager->setPlayerLight(glm::vec3(m_player->getPosition().x, 15.0f, m_player->getPosition().z), glm::vec4(1.0f, 1.0f, 1.0f, 25.0f));
	m_lightManager->setSun(glm::vec3(m_player->getPosition().x, 20.0f, m_player->getPosition().z), glm::vec3(0.8f, 0.8f, 0.8f));
	//for (int i = 0; i < 1; i++)
	//{
	//m_lightManager->addLight(
	//	// Position
	//	glm::vec3(
	//		Randomizer::single(-20.f, 20.f),
	//		40.f,
	//		Randomizer::single(-60.f, 20.f)),
	//	// Color
	//	glm::vec3(
	//		Randomizer::single(0.f, 255.f) / 255.f,
	//		Randomizer::single(0.f, 255.f) / 255.f,
	//		Randomizer::single(0.f, 255.f) / 255.f),
	//	60.f, m_gameObjectManager);

	//}

	//Used for the player flashlight & shadow mapping from the 
	//flashlights view
	m_renderer->preparePlayerLights(m_gameObjectManager->getPlayer());
}

void PlayState::addRoom()
{
	/*
	for (int i = 0; i <= 7; i++)
	{
		std::string file = "Room";
		std::string id = "Room";
		Type type = ROOM;

		if (i<10)
		{
			file += "0" + std::to_string(i);
			id += "0" + std::to_string(i);
		}
		else {
			file += std::to_string(i);
			id += std::to_string(i);
		}
		if (i==2)
		{
			type = ROOM_BOSS;
		}
		else if (i == 0) {
			type = ROOM_EMPTY;
		}
		*/

	FBXParserData* roomStart = m_FBXParser->binaryMeshReading(FBXPath + "roomStart.bin"); //SHOULD MAKE BINARY
	Mesh * roomMesh = m_GLinit->createMeshFBX("roomStart", roomStart);
	delete roomStart;
	Room * r_roomStart = new Room(roomMesh, ROOM_EMPTY, m_player);
	m_gameObjectManager->addGameObject(r_roomStart);

	FBXParserData* room2 = m_FBXParser->binaryMeshReading(FBXPath + "room2.bin"); //SHOULD MAKE BINARY
	Mesh * room2Mesh = m_GLinit->createMeshFBX("room2", room2);
	delete room2;
	Room * r_room2 = new Room(room2Mesh, ROOM, m_player);
	m_gameObjectManager->addGameObject(r_room2);

	FBXParserData* room3 = m_FBXParser->binaryMeshReading(FBXPath + "room3.bin"); //SHOULD MAKE BINARY
	Mesh * room3Mesh = m_GLinit->createMeshFBX("room3", room3);
	delete room3;
	Room * r_room3 = new Room(room3Mesh, ROOM, m_player);
	m_gameObjectManager->addGameObject(r_room3);

	FBXParserData* room4 = m_FBXParser->binaryMeshReading(FBXPath + "room4.bin"); //SHOULD MAKE BINARY
	Mesh * room4Mesh = m_GLinit->createMeshFBX("room4", room4);
	delete room4;
	Room * r_room4 = new Room(room4Mesh, ROOM, m_player);
	m_gameObjectManager->addGameObject(r_room4);

	FBXParserData* room5 = m_FBXParser->binaryMeshReading(FBXPath + "room5.bin"); //SHOULD MAKE BINARY
	Mesh * room5Mesh = m_GLinit->createMeshFBX("room5", room5);
	delete room5;
	Room * r_room5 = new Room(room5Mesh, ROOM, m_player);
	m_gameObjectManager->addGameObject(r_room5);

	FBXParserData* room6 = m_FBXParser->binaryMeshReading(FBXPath + "room6.bin"); //SHOULD MAKE BINARY
	Mesh * room6Mesh = m_GLinit->createMeshFBX("room6", room6);
	delete room6;
	Room * r_room6 = new Room(room6Mesh, ROOM, m_player);
	m_gameObjectManager->addGameObject(r_room6);

	FBXParserData* room7 = m_FBXParser->binaryMeshReading(FBXPath + "room7.bin"); //SHOULD MAKE BINARY
	Mesh * room7Mesh = m_GLinit->createMeshFBX("room7", room7);
	delete room7;
	Room * r_room7 = new Room(room7Mesh, ROOM, m_player);
	m_gameObjectManager->addGameObject(r_room7);

	FBXParserData* roomBossData = m_FBXParser->binaryMeshReading(FBXPath + "roomBoss.bin"); //SHOULD MAKE BINARY
	Mesh * roomBossMesh = m_GLinit->createMeshFBX("roomBoss", roomBossData);
	delete roomBossData;
	Room * r_roomBoss = new Room(roomBossMesh, ROOM_BOSS, m_player);
	m_gameObjectManager->addGameObject(r_roomBoss);

	//ParserData* roomStart = m_parser->loadFromObj(file + ".obj");
	//Mesh * roomMesh = m_GLinit->createMesh(id, roomStart);



	//DOOORS
	ParserData * doorData = m_parser->loadFromObj("Doors.obj");

	Mesh * door = m_GLinit->createMesh("Door", doorData);

	m_gameObjectManager->addGameObject(new Box(door, DOOR));


}
