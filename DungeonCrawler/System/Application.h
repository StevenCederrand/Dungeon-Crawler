#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "../Audio/AudioEngine.h"
#include "States/StateManager.h"
#include <Globals/Settings.h>
class Application
{
public:
	Application();
	~Application();

	bool initialize();
	void initShaders();
	void run();

	
private:
	void renderImGUI();

private:
	GLFWwindow* m_window;
	Input* m_input;
	StateManager* m_stateManager;
	AudioEngine* m_audioEngine;
	Settings m_settings;
	bool m_vsync;
};

#endif
