#include <GLM/gtx/transform.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "ShaderMap.h"
#include "../System/Log.h"
#include "Globals/Settings.h"
#define MESH_VECTOR_RESERVE_SIZE 150

Renderer::Renderer(Camera* camera, LightManager* lightManager, Effects* effects, Map* map)
{
	m_camera = camera;
	m_lightManager = lightManager;
	m_map = map;
	m_framebuffer = new Framebuffer();
	glEnable(GL_DEPTH_TEST);
	//Generate framebuffers & textures
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	this->m_framebuffer->genFrameBuffers();
	this->initRenderQuad();

	Shader* shader = ShaderMap::getShader("GeometryPass");
	shader->use();
	shader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());
	shader->unuse();

	//Create a new light projection matrix -- This doens't need to be created every frame
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(LIGHTFOV),
		(float)ScreenResolutionX / (float)ScreenResolutionY, NEAR_CLIP, FAR_CLIP);
	m_framebuffer->setProjectionMatrix(projectionMatrix);

	m_effects = effects; // Point to effect class
}

Renderer::~Renderer() {
	glDeleteBuffers(1, &this->m_rQuadVBO);
	glDeleteVertexArrays(1, &this->m_rQuadVAO);
	delete m_framebuffer;
}

void Renderer::prepareGameObjects(const std::vector<GameObject*>& gameObjects)
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		GameObject* object = gameObjects[i];

		m_meshIterator = m_meshes.find(object->getMesh());

		if (m_meshIterator != m_meshes.end())
		{
			m_meshIterator->second.emplace_back(object);
		}
		else
		{
			std::vector<GameObject*> meshVec;
			meshVec.reserve(MESH_VECTOR_RESERVE_SIZE);
			meshVec.emplace_back(object);
			m_meshes.emplace(object->getMesh(), meshVec);
		}
	}
}

void Renderer::preparePlayerLights(Player* player) {
	m_playerSpotLight = player->getSpotlight();
	m_playerLight = player->getFlash();

}

void Renderer::render() {


	this->shadowPass();
	this->geometryPass();
	this->lightPass();

	// Copy over the depthbuffer from the previous passes.
	glEnable(GL_DEPTH_TEST);
	m_framebuffer->bindFrameBuffer();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); //write to default framebuffer
	glBlitFramebuffer(
		0, 0, ScreenResolutionX, ScreenResolutionY, 0, 0, ScreenResolutionX, ScreenResolutionY, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	this->renderEffects();
	this->renderMap();
}

void Renderer::shadowPass() {
	if (m_meshes.size() == 0) {
		return;
	}

	this->configureShadowMapperVM();

	Shader* shadowShader = ShaderMap::getShader("ShadowPass");
	shadowShader->use();
	shadowShader->setMat4("lightSpaceMatrix", m_framebuffer->getLightSpaceMatrix());
	
	m_framebuffer->bindShadowBuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	for (auto mesh : m_meshes) {
		glBindVertexArray(mesh.first->getVao());
		glEnableVertexAttribArray(0);
		for (auto object : mesh.second) {

			if (dynamic_cast<Player*>(object)) continue;

			shadowShader->setMat4("modelMatrix", object->getModelMatrix());
			glDrawElements(GL_TRIANGLES, mesh.first->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
		}
		glDisableVertexAttribArray(0);
		glBindVertexArray(NULL);
	}
	m_framebuffer->unbindBuffer();
}

void Renderer::geometryPass() {
	if (m_meshes.size() == 0) {
		return;
	}

	//Use the geometry shader
	Shader* geometryShader = ShaderMap::getShader("GeometryPass");
	geometryShader->use();

	geometryShader->setMat4("viewMatrix", m_camera->getViewMatrix());

	m_framebuffer->bindFrameBuffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &mesh : this->m_meshes) {
		bindMesh(mesh.first, geometryShader);

		for (auto object : mesh.second) {
			geometryShader->setMat4("modelMatrix", object->getModelMatrix());
			geometryShader->setVec3("colorTint", object->getColorTint());
			glDrawElements(GL_TRIANGLES, mesh.first->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
		}

		unbindMesh(mesh.first);
	}
	geometryShader->unuse();
	this->m_meshes.clear();
		
	m_framebuffer->unbindBuffer();
}

void Renderer::renderEffects()
{
	glEnable(GL_BLEND);
	Shader* effectsShader = ShaderMap::getShader("EffectsShader");
	effectsShader->use();
	effectsShader->setMat4("viewMatrix", m_camera->getViewMatrix());
	effectsShader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());
	
	const std::map<std::string, Emitter*>& emitters = m_effects->getEmitters();
	
	for (const auto& map : emitters)
	{
		glBindVertexArray(map.second->getVAO());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, map.second->getTextureID());
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, map.second->getNumberOfParticles());
		glBindTexture(GL_TEXTURE_2D, NULL);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		glBindVertexArray(0);
	}

	effectsShader->unuse();
	glDisable(GL_BLEND);
}

void Renderer::lightPass() {

	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	Shader* lightShader = ShaderMap::getShader("LightPass");
	lightShader->use();
	if (m_playerSpotLight != nullptr) {
		lightShader->setVec3("spotlight.position", m_playerSpotLight->position);
		lightShader->setVec3("spotlight.direction", m_playerSpotLight->direction);
		lightShader->setFloat("spotlight.radius", m_playerSpotLight->radius);
		
		lightShader->setVec4("flashPosition", m_playerLight->position);
		lightShader->setVec4("flashColor", m_playerLight->color);
	}

	lightShader->setMat4("lightSpaceMatrix", m_framebuffer->getLightSpaceMatrix());
	lightShader->setInt("numberOfLights", m_lightManager->getNumberOfLights());
	lightShader->setVec3("cameraPosition", m_camera->getPosition());
	drawQuad();
	lightShader->unuse();
}

void Renderer::renderMap()
{
	Shader* mapShader = ShaderMap::getShader("MapShader");
	mapShader->use();
	
	std::vector<glm::vec4> maxMinValues = m_map->getRoomCoordinates();
	float tempValue = float(1) / float(45);

	mapShader->setMat4("viewMatrix", m_camera->getViewMatrix());
	mapShader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());

	for (size_t i = 0; i < maxMinValues.size(); i++)
	{
		glm::mat4 modelmatrix = glm::mat4(1.0f);
		modelmatrix = glm::translate(modelmatrix, glm::vec3(0.0f, 10.0f, 0.0f));
		
		modelmatrix = glm::translate(modelmatrix, glm::vec3(
			maxMinValues[i].x * tempValue,
			-maxMinValues[i].y * tempValue,
			0.0f));

		modelmatrix = glm::scale(modelmatrix, glm::vec3(1.0f, 1.0f, 0.f));

		mapShader->setMat4("modelMatrix", modelmatrix);
		glBindVertexArray(m_map->getVao());
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 12);

	}

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);






	mapShader->unuse();
}

void Renderer::bindMesh(Mesh * mesh, Shader* shader)
{
	glBindVertexArray(mesh->getVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Diffuse texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID());
	//Bind normal texture
	if (mesh->hasNormalMap()) {
		shader->setInt("hasNormalMap", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh->getNormalID());	
	}
	else {
		shader->setInt("hasNormalMap", 0);
	}
	//Bind AO texture
	if (mesh->hasAmbientMap()) {
		shader->setVec3("cameraPosition", m_camera->getPosition());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mesh->getAmbientID());
	}
}

void Renderer::unbindMesh(Mesh * mesh)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NULL);

	if (mesh->hasNormalMap()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
	if (mesh->hasAmbientMap()) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
	glBindVertexArray(NULL);
}

bool Renderer::initRenderQuad() {
	glGenVertexArrays(1, &this->m_rQuadVAO);
	glGenBuffers(1, &this->m_rQuadVBO);

	glBindVertexArray(this->m_rQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_rQuadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_rQuadData), &m_rQuadData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	return false;
}

void Renderer::drawQuad() {
	glBindVertexArray(this->m_rQuadVAO);
	m_framebuffer->bindDeferredTextures();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::configureShadowMapperVM() {
	glm::vec3 pos = m_playerSpotLight->position + glm::vec3(0, 1.f, 0);
	glm::mat4 viewMatrix = glm::lookAt(pos, pos + m_playerSpotLight->direction, glm::vec3(0, 1, 0));
	m_framebuffer->setViewMatrix(viewMatrix);
}
