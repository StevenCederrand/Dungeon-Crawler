#ifndef FRAMEBUFFER_h
#define FRAMEBUFFER_h
#include <GL/glew.h>
#include <GLM/glm.hpp>

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	void genFrameBuffers();
	void bindShadowBuffer();
	void bindFrameBuffer();
	void bindDeferredTextures();

	void unbindBuffer();
	void unbindDeferredTextures();
	unsigned int getFrameBuffer() const;

	void setProjectionMatrix(const glm::mat4& projectionMatrix);
	const glm::mat4& getProjectionMatrix() const;

	void setViewMatrix(const glm::mat4& viewMatrix);
	const glm::mat4& getViewMatrix() const;
	
	const glm::mat4& getLightSpaceMatrix();
private:
	void genDeferredBuffers();
	void genShadowMappingBuffers();
	void genRenderBuffer();

private:
	unsigned int m_frameBuffer;
	unsigned int m_rbo;
	unsigned int m_shadowBuffer; //This is my depthMapFBO

	unsigned int m_depthMap; //Depth map texture
	unsigned int m_colourBuffer;
	unsigned int m_normalBuffer;
	unsigned int m_positionBuffer;

	//Projection and view matrix for shadow mapping
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix; 
};


#endif
