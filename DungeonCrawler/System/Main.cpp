#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"
#include "Log.h"

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	Log::initialize();

	Application* app = new Application();

	bool appInitialized = app->initialize();

	if (appInitialized)
		app->run();
	else 
		system("pause");
	
	delete app;
	
	return 0;
}