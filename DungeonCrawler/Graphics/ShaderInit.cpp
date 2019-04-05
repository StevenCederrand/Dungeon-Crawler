#include "ShaderInit.h"

ShaderInit::ShaderInit() {
	
	Shader* shader = ShaderMap::createShader("GameObjectShader", "GameObjectShader.vs", "GameObjectShader.fs");
	shader = ShaderMap::createShader("MenuShader", "MainMenu/MainMenu.vs", "MainMenu/MainMenu.fs");
	shader = ShaderMap::createShader("GeometryPass", "GeometryPass/GeometryPass.vs", "GeometryPass/GeometryPass.fs");
	shader = ShaderMap::createShader("LightPass", "LightPass/LightPass.vs", "LightPass/LightPass.fs");
	shader->use();
	shader->setInt("positionBuffer", 0);
	shader->setInt("normalBuffer", 1);
	shader->setInt("colourBuffer", 2);
	shader->unuse();
}

ShaderInit::~ShaderInit()
{
}
