#include "Renderer.h"

#define MESH_VECTOR_RESERVE_SIZE 150

Renderer::Renderer()
{
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

	//if (m_meshes.size() == 0)
	//	return;

	//// Use shader
	//// Set view matrix
	//// Set projectionMatrix (if not already set)

	//for (auto &currentMesh : m_meshes)
	//{
	//	bindMesh(currentMesh.first);

	//	for (auto object : currentMesh.second)
	//	{
	//		// Set per gameobject attributes to shader
	//		// Model matrix for an example
	//		glDrawElements(GL_TRIANGLES, object->getMesh()->getNrOfIndices(), GL_UNSIGNED_INT, NULL);
	//	}

	//	unbindMesh();

	//}

	//// Unuse shader

	//// clear mesh map
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
