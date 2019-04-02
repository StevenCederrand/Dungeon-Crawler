#include "Application.h"
#include "States/MenuState.h"
#include <iostream>

#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_glfw.h"
#include "Vendor/ImGui/imgui_impl_opengl3.h"

#include "Log.h"
#include "Graphics/MeshMap.h"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

int Application::windowWidth = 1280;
int Application::windowHeight = 720;

Application::Application()
{
}


Application::~Application()
{
	MeshMap::cleanUp();
	delete m_input;
	delete m_stateManager;
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

	m_window = glfwCreateWindow(Application::windowWidth, Application::windowHeight, "Dungeon Crawler", NULL, NULL);

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
	glfwSwapInterval(1);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
	ImGui::StyleColorsDark();

	// Initializes the input sytem
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
		
		m_input->clearKeys();
		glfwPollEvents();

		currentTime = static_cast<float>(glfwGetTime());
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		m_stateManager->update(dt);
		m_stateManager->render();

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
	int totalGpuMemory;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
		&totalGpuMemory);

	int availableGpuMemory;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
		&availableGpuMemory);

	ImGui::Begin("System");

	// fps
	float fps = ImGui::GetIO().Framerate;
	ImGui::TextColored(ImVec4(1.f, 1.0f, 1.f, 1.0f), "FPS: %.1f", fps);

	// Gpu data
	ImGui::TextColored(ImVec4(204.f / 255.f, 102.f / 255.f, 0.f, 1.0f),
		"Total GPU Memory: %i%s\n%s%i%s\n%s%i%s", totalGpuMemory / 1024, " MB", "Available GPU Memory: ", availableGpuMemory / 1024, " MB",
		"Used GPU Memory: ", (totalGpuMemory / 1024) - (availableGpuMemory / 1024), " MB");

	ImGui::End();
}
