#include "Renderer.h"
#include "ShaderMap.h"
#include "../System/Log.h"
#define MESH_VECTOR_RESERVE_SIZE 150

Renderer::Renderer(Camera* camera, LightManager* lightManager)
{
	m_camera = camera;
	m_lightManager = lightManager;
	m_framebuffer = new Framebuffer();
	glEnable(GL_DEPTH_TEST);
	//Generate framebuffers & textures
	
	if (m_framebuffer->genFrameBuffers() != FRAMEBUFFER_OK) {
		LOG_ERROR("FRAMEBUFFER FAILED");
	}
	this->initRenderQuad();
	Shader* shader = ShaderMap::getShader("GeometryPass");

	shader->use();
	shader->setMat4("projectionMatrix", m_camera->getProjectionMatrix());
	shader->unuse();
}

Renderer::~Renderer()
{
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

void Renderer::render()
{
	this->geometryPass(); 
	this->lightPass();	
}

void Renderer::forwardPass() {
	//if (m_meshes.size() == 0)
	//	return;

	//// Use shader
	//Shader* goShader = ShaderMap::getShader("GameObjectShader");
	//goShader->use();

	//// Set view matrix
	//goShader->setMat4("viewMatrix", m_camera->getViewMatrix());
	//goShader->setVec3("cameraPosition", m_camera->getPosition());

	//for (auto &currentMesh : m_meshes)
	//{
	//	bindMesh(currentMesh.first, goShader);

	//	for (auto object : currentMesh.second)
	//	{
	//		goShader->setMat4("modelMatrix", object->getModelMatrix());
	//		goShader->setFloat("shininess", object->getMesh()->getShininess());
	//		glDrawElements(GL_TRIANGLES, object->getMesh()->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
	//	}

	//	unbindMesh();
	//}

	//// Unuse shader
	//goShader->unuse();

	//// clear mesh map
	//m_meshes.clear();

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
			glDrawElements(GL_TRIANGLES, object->getMesh()->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
		}

		unbindMesh();
	}
	geometryShader->unuse();
	this->m_meshes.clear();
	
	m_framebuffer->unbindBuffer();
}

void Renderer::lightPass() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	Shader* lightShader = ShaderMap::getShader("LightPass");
	lightShader->use();
	lightShader->setInt("numberOfLights", m_lightManager->getNumberOfLights());
	lightShader->setVec3("cameraPosition", m_camera->getPosition());
	drawQuad();
	lightShader->unuse();
}

void Renderer::bindMesh(Mesh * mesh, Shader* shader)
{
	glBindVertexArray(mesh->getVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
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

void Renderer::drawQuad() {
	glBindVertexArray(this->m_rQuadVAO);
	m_framebuffer->bindDeferredTextures();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}