#ifndef FRAMEBUFFER_h
#define FRAMEBUFFER_h
#include <GL/glew.h>
#include "../Globals/Settings.h"

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();


	void genFrameBuffers();
	void genDeferredBuffers();
	void genShadowMappingBuffers();
	void genRenderBuffer();

	void bindShadowBuffer();
	void bindFrameBuffer();
	
	void bindDeferredTextures();
	

	void unbindBuffer();
	void unbindDeferredTextures();

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