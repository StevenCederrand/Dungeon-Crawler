#ifndef UIRENDERER_h
#define UIRENDERER_h
#include "../Utility/Camera.h"
#include "../Main Menu/MainMenu.h"
#include "ShaderMap.h"
class UIRenderer {
public:
	UIRenderer(Camera* camera);
	~UIRenderer();

	void render();
	void setupMenuButtons(MainMenu* menu);


private: 
	//These are deleted in the menu state
	Camera* m_camera;
	MainMenu* m_mainMenu;


};

#endif