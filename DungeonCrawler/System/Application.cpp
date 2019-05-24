#include "Application.h"
#include "States/MenuState.h"
#include <iostream>

#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_glfw.h"
#include "Vendor/ImGui/imgui_impl_opengl3.h"

#include "Log.h"
#include "Graphics/MeshMap.h"
#include "Graphics/ShaderMap.h"




Application::Application()
{

}

Application::~Application() {
	MeshMap::cleanUp();
	ShaderMap::cleanUp();
	delete m_input;
	delete m_stateManager;
	delete m_audioEngine;

	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

bool Application::initialize()
{

	bool glfwInitialized = glfwInit();

	if (!glfwInitialized) {
		LOG_ERROR("Failed to initialize GLFW");
	}

	// Using openGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	Settings::setResolution(1280, 720);
	Settings::setMaxLights(25);

	m_window = glfwCreateWindow(Settings::getScreenWidth(), Settings::getScreenHeight(), "Dungeon Crawler", NULL, NULL);

	if (m_window == nullptr) {
		glfwTerminate();
		LOG_ERROR("Failed to create GLFW window");
		return false;
	}

	// Opengl context
	glfwMakeContextCurrent(m_window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		glfwTerminate();
		LOG_ERROR("Failed to initialize GLEW");
	}

	// Vsync
	m_vsync = true;
	glfwSwapInterval(1);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
	ImGui::StyleColorsDark();

	// Initialize the audio system
	m_audioEngine = new AudioEngine();

	// Initialize the shaders
	this->initShaders();

	// Initializes the input system
	m_input = new Input();
	m_stateManager = new StateManager();
	m_stateManager->setState(new MenuState());

	

	LOG_INFO("Application successfully initialized");
	return true;
}

void Application::run()
{
	float currentTime = 0.f;
	float lastTime = 0.f;
	LOG_INFO("Running Application loop");

	while (!glfwWindowShouldClose(m_window)) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_input->clearKeys();
		glfwPollEvents();

		if (Input::isKeyReleased(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(m_window, true);
		}
		currentTime = static_cast<float>(glfwGetTime());
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		m_stateManager->update(dt);
		m_stateManager->render();

		if (m_stateManager->wasStateChanged())
		{
			lastTime = static_cast<float>(glfwGetTime());
			m_stateManager->resetWasStateChanged();
		}


		// SOUND STUFF
		this->m_audioEngine->update();

		// IMGUI STUFF
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		this->renderImGUI();
		m_stateManager->renderImGUI();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window);
	}
}


void Application::renderImGUI()
{
	ImGui::Begin("System (Esc to exit application)");

	// fps
	float fps = ImGui::GetIO().Framerate;
	ImGui::TextColored(ImVec4(1.f, 1.0f, 1.f, 1.0f), "FPS: %.1f", fps);

	ImGui::Checkbox("Vsync ", &m_vsync);

	if (m_vsync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
	

	ImGui::End();
}

void Application::initShaders() {
	Shader* shader = ShaderMap::createShader("GameObjectShader", "GameObjectShader.vs", "GameObjectShader.fs");
	shader = ShaderMap::createShader("MenuShader", "MainMenu/MainMenu.vs", "MainMenu/MainMenu.fs");
	shader = ShaderMap::createShader("GeometryPass", "GeometryPass/GeometryPass.vs", "GeometryPass/GeometryPass.gs", "GeometryPass/GeometryPass.fs");
	shader = ShaderMap::createShader("LightPass", "LightPass/LightPass.vs", "LightPass/LightPass.fs");
	shader = ShaderMap::createShader("ShadowPass", "ShadowPass/ShadowPass.vs", "ShadowPass/ShadowPass.fs");
	shader = ShaderMap::createShader("EffectsShader", "EffectsShader.vs", "EffectsShader.fs");
	shader = ShaderMap::createShader("AnimatedEffectsShader", "AnimatedEffects.vs", "AnimatedEffects.fs");
	shader = ShaderMap::createShader("UIShader", "UI.vs", "UI.fs");
	shader = ShaderMap::createShader("MapShader", "Map.vs", "Map.fs");
	shader = ShaderMap::createShader("ScreenBloodShader", "ScreenBlood.vs", "ScreenBlood.fs");
}



