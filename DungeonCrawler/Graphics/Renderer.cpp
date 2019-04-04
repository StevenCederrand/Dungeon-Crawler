#include "Renderer.h"
#include "ShaderMap.h"
#include "../System/Log.h"
#define MESH_VECTOR_RESERVE_SIZE 150

Renderer::Renderer(Camera* camera)
{
	m_camera = camera;
	glEnable(GL_DEPTH_TEST);
	//Generate framebuffers & textures
	
	if (this->m_framebuffer.genFrameBuffers() != FRAMEBUFFER_OK) {
		LOG_ERROR("FRAMEBUFFER FAILED");
	}

	if (!this->initRenderQuad()) {

	}
}

Renderer::~Renderer()
{
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

void Renderer::render()
{

	/*
		Write a geometry pass shader
	*/
	this->geometryPass(); 
	
	
}


void Renderer::forwardPass() {
	if (m_meshes.size() == 0)
		return;

	// Use shader
	Shader* goShader = ShaderMap::getShader("GameObjectShader");
	goShader->use();

	// Set view matrix
	goShader->setMat4("viewMatrix", m_camera->getViewMatrix());
	goShader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());

	for (auto &currentMesh : m_meshes)
	{
		bindMesh(currentMesh.first);

		for (auto object : currentMesh.second)
		{
			goShader->setMat4("modelMatrix", object->getModelMatrix());
			glDrawElements(GL_TRIANGLES, object->getMesh()->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
		}

		unbindMesh();

	}

	// Unuse shader
	goShader->unuse();

	// clear mesh map
	m_meshes.clear();

}

void Renderer::geometryPass() {
	if (m_meshes.size() == 0) {
		return;
	}

	//Use the geometry shader
	Shader* geometryShader = ShaderMap::getShader("GeometryShader");
	geometryShader->use();

	geometryShader->setMat4("viewMatrix", m_camera->getViewMatrix());
	geometryShader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());

	this->m_framebuffer.bindFrameBuffer();
	
	for (auto &mesh : this->m_meshes) {
		bindMesh(mesh.first);

		for (auto object : mesh.second) {
			geometryShader->setMat4("modelMatrix", object->getModelMatrix());
			glDrawElements(GL_TRIANGLES, object->getMesh()->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
		}

		unbindMesh();
	}
	this->m_framebuffer.unbindBuffer();
	
	geometryShader->unuse();
	this->m_meshes.clear();
}

void Renderer::lightPass() {
	Shader* lightShader = ShaderMap::getShader("LightShader");
	lightShader->use();
	glClear(GL_COLOR_BUFFER_BIT);
	//Insert light pass code


	glBindVertexArray(this->m_rQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_rQuadVBO);
	this->m_framebuffer.bindDeferredTextures();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::bindMesh(Mesh * mesh)
{
	glBindVertexArray(mesh->getVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID());
}

void Renderer::unbindMesh()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindTexture(GL_TEXTURE_2D, NULL);
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