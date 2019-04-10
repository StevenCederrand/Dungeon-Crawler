#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Log.h"
#include "System/Input.h"
#include "StateManager.h"

#include "Graphics/MeshMap.h"
#include "Graphics/ShaderMap.h"
#include "GameObjects/Box.h"
#include "GameObjects/Player.h"


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
	ParserData* boxData = m_parser->loadFromObj("box.obj");
	ParserData* sphereData = m_parser->loadFromObj("sphere.obj");

	Mesh* boxMesh = m_GLinit->createMesh("Box", boxData);
	m_gameObjectManager->addGameObject(new Box(boxMesh));
	m_gameObjectManager->addGameObject(new Box(boxMesh, glm::vec3(0.f, -4.f, 0.f)));
	m_gameObjectManager->addGameObject(new Box(boxMesh, glm::vec3(2.f, -4.f, 0.f)));
	m_gameObjectManager->addGameObject(new Player(boxMesh));

	m_GLinit->createMesh("Sphere", sphereData);
	#pragma endregion
	
	m_lightManager->setSun(ShaderMap::getShader("LightPass"), glm::vec3(-5.f, 1.5f, 0.f), glm::vec3(0.8f, .8f, 0.8f));
	m_lightManager->addLight(glm::vec3(5.f), glm::vec3(0.5f, 0.f, 1.f), 10.f, m_gameObjectManager);
	m_lightManager->addLight(glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.0f, 1.f, 0.f), 10.f, m_gameObjectManager);

	
}

PlayState::~PlayState() {
	delete m_parser;
	delete m_GLinit;
	delete m_camera;
	delete m_gameObjectManager;
	delete m_renderer;
	delete m_lightManager;
}

void PlayState::update(float dt)
{
	m_camera->update(dt);
	m_gameObjectManager->update(dt);
	m_lightManager->update(dt);

	m_renderer->prepareGameObjects(m_gameObjectManager->getGameObjects());

	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT)){
		m_stateManager->popState();
	}
}


void PlayState::renderImGUI()
{
	ImGui::Begin("PlayState");

	ImGui::Text("Press right mouse to switch back to menu state");

	ImGui::End();
}

void PlayState::render()
{
	m_renderer->render();
}
