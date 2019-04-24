#ifndef _UIManager_H
#define _UIManager_H
#include "UI.h"
#include <Utility/UICamera.h>
#include <vector>

class UIManager {
public:
	UIManager(UICamera* camera);
	~UIManager();

	void update(float dt);
	void render();

	void registerUIElement(UI* element);

private:
	std::vector<UI*> m_uiElements;
	UICamera* m_camera;
	Shader* m_shader;
};

#endif
