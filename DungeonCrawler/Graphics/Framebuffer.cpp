#include "Framebuffer.h"
#include "../System/Log.h"

Framebuffer::Framebuffer() {
}

Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &m_colourBuffer);
	glDeleteTextures(1, &m_normalBuffer);
	glDeleteTextures(1, &m_positionBuffer);
	glDeleteTextures(1, &m_depthMap); 
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &this->m_frameBuffer);
	glDeleteFramebuffers(1, &this->m_shadowBuffer);
}

void Framebuffer::genFrameBuffers() {
	
	//Deferred rendering 
	glGenFramebuffers(1, &this->m_frameBuffer);
	this->bindFrameBuffer();
	this->genDeferredBuffers();
	this->genRenderBuffer();
	this->unbindBuffer();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG_ERROR("FRAMEBUFFER INCOMPLETE");
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	//Used for shadow mapping
	glGenFramebuffers(1, &this->m_shadowBuffer);
	this->bindShadowBuffer();
	this->genShadowMappingBuffers();
	this->unbindBuffer();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG_ERROR("ERROR GENERATING SHADOW FRAMEBUFFER OBJECT");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

void Framebuffer::genRenderBuffer() {
	glGenRenderbuffers(1, &this->m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
								ScreenResolutionX, ScreenResolutionY);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_rbo);
}

void Framebuffer::bindShadowBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_shadowBuffer);
}

void Framebuffer::bindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_frameBuffer);
}

void Framebuffer::unbindBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

void Framebuffer::genDeferredBuffers() {
	//Generate position texture
	glGenTextures(1, &this->m_positionBuffer);
	glBindTexture(GL_TEXTURE_2D, this->m_positionBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ScreenResolutionX,
		ScreenResolutionY, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_positionBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, NULL);

	//Generate normal texture
	glGenTextures(1, &this->m_normalBuffer);
	glBindTexture(GL_TEXTURE_2D, this->m_normalBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenResolutionX,
		ScreenResolutionY, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->m_normalBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, NULL);

	//Generate colour texture
	glGenTextures(1, &this->m_colourBuffer);
	glBindTexture(GL_TEXTURE_2D, this->m_colourBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenResolutionX, ScreenResolutionY, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->m_colourBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, NULL);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
}

void Framebuffer::genShadowMappingBuffers() {
	glGenTextures(1, &this->m_depthMap);
	glBindTexture(GL_TEXTURE_2D, this->m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ScreenResolutionX, ScreenResolutionY,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	
	//Set border color to white
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//Generate depthmap texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

void Framebuffer::bindDeferredTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->m_positionBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->m_normalBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->m_colourBuffer);
	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, this->m_depthMap);
}

void Framebuffer::unbindDeferredTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, NULL);
	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, NULL);
}

unsigned int Framebuffer::getFrameBuffer() const {
	return this->m_frameBuffer;
}

void Framebuffer::setProjectionMatrix(const glm::mat4 & projectionMatrix) {
	m_projectionMatrix = projectionMatrix;
}

const glm::mat4 & Framebuffer::getProjectionMatrix() const {
	return m_projectionMatrix;
}

void Framebuffer::setViewMatrix(const glm::mat4 & viewMatrix) {
	m_viewMatrix = viewMatrix;
}

const glm::mat4 & Framebuffer::getViewMatrix() const {
	return m_viewMatrix;
}

const glm::mat4 & Framebuffer::getLightSpaceMatrix() {

	return m_projectionMatrix * m_viewMatrix;
}

