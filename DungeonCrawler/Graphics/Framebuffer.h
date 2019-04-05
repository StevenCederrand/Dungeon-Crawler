#ifndef FRAMEBUFFER_h
#define FRAMEBUFFER_h
#include <GL/glew.h>
#include "../Globals/Settings.h"

static enum FRAMEBUFFER_RESULT {
	OK,
	FRAMEBUFFER_OK,
	FRAMEBUFFER_FAILED,
	RENDERBUFFER_OK,
	RENDERBUFFER_FAILED,
	TEXTURES_OK,
	TEXTURES_FAILED,
	SHADOWMAP_OK,
	SHADOWMAP_FAILED,
	DEPTHMAP_OK,
	DEPTHMAP_FAILED
};

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	FRAMEBUFFER_RESULT genFrameBuffers();
	void bindShadowBuffer();
	void bindFrameBuffer();
	void bindDeferredTextures();
	
	void unbindBuffer();
	void unbindDeferredTextures();
	unsigned int getFrameBuffer() const;
	FRAMEBUFFER_RESULT frameBufferStatus();

private:
	void genDeferredBuffers();
	void genShadowMappingBuffers();
	void genRenderBuffer();

private:
	unsigned int m_frameBuffer;
	unsigned int m_rbo;
	unsigned int m_shadowBuffer;


	unsigned int m_depthMap; //Related to shadow mapping
	unsigned int m_colourBuffer;
	unsigned int m_normalBuffer;
	unsigned int m_positionBuffer;


};


#endif