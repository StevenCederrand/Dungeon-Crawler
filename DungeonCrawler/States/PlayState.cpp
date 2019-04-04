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

	Shader* goShader = ShaderMap::getShader("GeometryPass");
	goShader->use();
	goShader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());
	goShader->unuse();

	Shader* lightShader = ShaderMap::getShader("LightPass");
	lightShader->use();
	// Temporary sun creation
	glm::vec3 sunColor = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 sunPosition = glm::vec3(-5.f, 1.5f, 0.f);
	lightShader->setVec3("sunColor", sunColor);
	lightShader->setVec3("sunPosition", sunPosition);
	lightShader->unuse();

	m_lightManager = new LightManager();
	m_lightManager->addLight(glm::vec3(1.f), glm::vec3(0.f, 1.f, 0.f), 10.f);

	m_gameObjectManager->addGameObject(new Box(boxMesh));
	m_gameObjectManager->addGameObject(new Box(boxMesh, glm::vec3(0.f,-4.f,0.f)));
	m_gameObjectManager->addGameObject(new Box(boxMesh, glm::vec3(2.f, -4.f, 0.f)));
}

PlayState::~PlayState()
{
	LOG_WARNING("PlayState destroyed");

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
