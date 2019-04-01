#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"
#include <FMOD/fmod.hpp>

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Application* app = new Application();

	FMOD_RESULT res;
	FMOD::System* sys;
	res = FMOD::System_Create(&sys);
	if (res != FMOD_OK) {
		std::printf("ERROR CREATING FMOD SYSTEM");
		return -1;
	}

	bool appInitialized = app->initialize();

	if (appInitialized)
		app->run();
	else 
		system("pause");
	
	delete app;
	
	return 0;
}