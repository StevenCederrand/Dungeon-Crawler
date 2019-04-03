#include "Renderer.h"
#include "ShaderMap.h"

#define MESH_VECTOR_RESERVE_SIZE 150

Renderer::Renderer(Camera* camera)
{
	m_camera = camera;
	glEnable(GL_DEPTH_TEST);

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
