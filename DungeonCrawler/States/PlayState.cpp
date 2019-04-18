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

#include "Utility/Randomizer.h"
#include <chrono>

PlayState::PlayState() {
	auto start = std::chrono::steady_clock::now();

	m_parser = new Parser();
	m_GLinit = new GLinit();

	#pragma region Init
	m_camera = new Camera();
	Camera::active = m_camera;
	m_lightManager = new LightManager()	;
	m_renderer = new Renderer(m_camera, m_lightManager);
	m_gameObjectManager = new GameObjectManager();
	AudioEngine::loadSSO("Game.sso");
	#pragma endregion

	auto end = std::chrono::steady_clock::now();
	LOG_INFO("Init Time: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()));

	start = std::chrono::steady_clock::now();
	#pragma region Create_Objects
	ParserData* boxData = m_parser->loadFromObj("collisionboxtest.obj");
	ParserData* roomData = m_parser->loadFromObj("basementleveltest.obj");
	ParserData* sphereData = m_parser->loadFromObj("sphere.obj");

	m_GLinit->createMesh("Box", boxData);
	m_GLinit->createMesh("Room", roomData);
	m_GLinit->createMesh("Sphere", sphereData);
	#pragma endregion

	end = std::chrono::steady_clock::now();
	LOG_INFO("Create_objects Time: " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()));

	Mesh* roomMesh = MeshMap::getMesh("Room");
	Mesh* boxMesh = MeshMap::getMesh("Box");

	m_lightManager->setSun(ShaderMap::getShader("LightPass"), glm::vec3(-5.f, 1.5f, 0.f), glm::vec3(0.8f, .8f, 0.8f));

	m_lightManager->addLight(glm::vec3(0.f, 5.f, -5.f), glm::vec3(0.0f, 1.f, 0.f), 10.f, m_gameObjectManager);

	m_gameObjectManager->addGameObject(new Room(roomMesh, glm::vec3(0.f, 0.f, 0.f)));

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

	for (int i = 0; i < 20; i++)
	{
		m_gameObjectManager->addGameObject(new Box(boxMesh,
			glm::vec3(
				Randomizer::single(-15.f, 15.f),
				0.f,
				Randomizer::single(-30.f, 30.f)
			)));
	}

	m_player = new Player(boxMesh);
	m_gameObjectManager->addGameObject(m_player);

	//Used for the player flashlight & shadow mapping from the 
	//flashlights view
	m_renderer->prepareFlashlight(m_gameObjectManager->getPlayer());
}

PlayState::~PlayState() {
	delete m_parser;
	delete m_GLinit;
	delete m_camera;
	delete m_gameObjectManager;
	delete m_renderer;
	delete m_lightManager;
}

void PlayState::update(float dt) {

	m_gameObjectManager->update(dt);
	m_camera->update(dt);
	m_lightManager->update(dt);

	m_renderer->prepareGameObjects(m_gameObjectManager->getGameObjects());
}


void PlayState::renderImGUI()
{
	ImGui::Begin("PlayState");

	ImGui::Text("Press shift to DASH");

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
