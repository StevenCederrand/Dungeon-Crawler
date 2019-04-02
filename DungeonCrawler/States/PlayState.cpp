#include "PlayState.h"
#include "Vendor/ImGui/imgui.h"

#include "System/Log.h"
#include "System/Input.h"
#include "StateManager.h"

#include "Graphics/MeshMap.h"

PlayState::PlayState()
{
	LOG_INFO("PlayState created");
	
	m_parser = new Parser();
	m_GLinit = new GLinit();

	ParserData* data = m_parser->loadFromObj("box.obj");
	m_GLinit->createMesh("Box", data);

	Mesh* box = MeshMap::getMesh("Box");

}

PlayState::~PlayState()
{
	LOG_WARNING("PlayState destroyed");
	delete m_parser;
	delete m_GLinit;
}

void PlayState::update(float dt)
{
	if (Input::isMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
	{
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
}
