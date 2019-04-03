#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Log.h"
#include "System/Input.h"
#include "StateManager.h"

#include "Graphics/MeshMap.h"
#include "Graphics/ShaderMap.h"
#include "GameObjects/Box.h"


PlayState::PlayState()
{
	LOG_INFO("PlayState created");
	
	m_parser = new Parser();
	m_GLinit = new GLinit();
	m_camera = new Camera();
	m_renderer = new Renderer(m_camera);
	m_gameObjectManager = new GameObjectManager();

	ParserData* data = m_parser->loadFromObj("box.obj");

	m_GLinit->createMesh("Box", data);

	Mesh* boxMesh = MeshMap::getMesh("Box");
	ShaderMap::createShader("GameObjectShader", "GameObjectShader.vert", "GameObjectShader.frag");

	m_gameObjectManager->addGameObject(new Box(boxMesh));
}

PlayState::~PlayState()
{
	LOG_WARNING("PlayState destroyed");

	delete m_parser;
	delete m_GLinit;
	delete m_camera;
	delete m_gameObjectManager;
	delete m_renderer;
}

void PlayState::update(float dt)
{
	m_camera->update(dt);
	m_gameObjectManager->update(dt);

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
