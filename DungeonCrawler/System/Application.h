#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "../Globals/Settings.h"
#include "../Audio/AudioEngine.h"
#include "States/StateManager.h"

class Application
{
public:
	Application();
	~Application();

	bool initialize();
	void run();

	static int windowWidth;
	static int windowHeight;

private:
	void renderImGUI();

private:
	GLFWwindow* m_window;
	Input* m_input;
	StateManager* m_stateManager;
	AudioEngine* m_audioEngine;
};

#endif
