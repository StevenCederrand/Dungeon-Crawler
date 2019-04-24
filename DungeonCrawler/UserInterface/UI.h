#ifndef _UI_H
#define _UI_H
#include <Graphics/Shader.h>

class UI {
public:
	UI(){}
	virtual ~UI(){}
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual const glm::mat4& getModelMatrix() const = 0;
};

#endif
