#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Log.h"
#include "System/Input.h"
#include "StateManager.h"

#include "Graphics/MeshMap.h"
#include "Graphics/ShaderMap.h"
#include "GameObjects/Box.h"
#include "GameObjects/Room.h"
#include "GameObjects/Player.h"
#include "GameObjects/Enemies/Walker.h"
#include "GameObjects/Enemies/Shooter.h"

#include "Utility/Randomizer.h"


PlayState::PlayState() {
	
	#pragma region Init
	m_parser = new Parser();
	m_GLinit = new GLinit();
	m_camera = new Camera();
	Camera::active = m_camera;
	m_lightManager = new LightManager();
	m_renderer = new Renderer(m_camera, m_lightManager);
	m_gameObjectManager = new GameObjectManager();
	#pragma endregion

	#pragma region Create_Objects
	ParserData* boxData = m_parser->loadFromObj("collisionboxtest.obj");
	//ParserData* roomData = m_parser->loadFromObj("basementleveltest.obj");
	//ParserData* roomData = m_parser->loadFromObj("oneRoomAi.obj");
	ParserData* roomData = m_parser->loadFromObj("roomWithNodes.obj");
	ParserData* sphereData = m_parser->loadFromObj("sphere.obj");

	m_GLinit->createMesh("Box", boxData);
	m_GLinit->createMesh("Room", roomData);
	m_GLinit->createMesh("Sphere", sphereData);
	#pragma endregion
	
	Mesh* roomMesh = MeshMap::getMesh("Room");
	Mesh* boxMesh = MeshMap::getMesh("Box");
	
	#pragma endregion
	
	m_lightManager->setSun(ShaderMap::getShader("LightPass"), glm::vec3(-5.f, 1.5f, 0.f), glm::vec3(0.8f, .8f, 0.8f));
	

	m_lightManager->addLight(glm::vec3(5.f), glm::vec3(0.5f, 0.f, 1.f), 10.f, m_gameObjectManager);
	m_lightManager->addLight(glm::vec3(0.f, 5.f, -5.f), glm::vec3(0.0f, 1.f, 0.f), 10.f, m_gameObjectManager);

	m_gameObjectManager->addGameObject(new Room(roomMesh, ROOM, glm::vec3(0.f, 0.f, 0.f)));

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

	//check the collsiion and then write to binary
	for (int i = 0; i < 20; i++)
	{
		m_gameObjectManager->addGameObject(new Box(boxMesh, BOX,
			glm::vec3(
				Randomizer::single(-15.f, 15.f), 
				0.f, 
				Randomizer::single(-30.f, 30.f)
			)));
	}
	
	m_gameObjectManager->nodecollision(roomData);
	m_parser->writeToBinary();

	m_shooter = new Shooter(boxMesh, SHOOTER);
	m_gameObjectManager->addGameObject(m_shooter);
	m_walker = new Walker(boxMesh, WALKER);
	m_gameObjectManager->addGameObject(m_walker);
	m_player = new Player(boxMesh, PLAYER);
	m_gameObjectManager->addGameObject(m_player);
}

PlayState::~PlayState() 
{
	delete m_parser;
	delete m_GLinit;
	delete m_camera;
	delete m_gameObjectManager;
	delete m_renderer;
	delete m_lightManager;
}

void PlayState::update(float dt)
{
	
	m_gameObjectManager->update(dt);
	m_camera->update(dt);
	m_lightManager->update(dt);

	m_renderer->prepareGameObjects(m_gameObjectManager->getGameObjects());
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


	ImGui::End();
}

void PlayState::render()
{
	m_renderer->render();
}
