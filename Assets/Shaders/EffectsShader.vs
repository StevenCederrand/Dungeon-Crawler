#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 center;
layout(location = 3) in vec4 color;
layout(location = 4) in float animationIndex;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float nrAnimationFrames;

out vec4 frag_color;
out vec2 frag_uv;

void calculateAnimatedUV();

void main()
{
	
	vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	vec3 vertexPosition_worldspace = center + cameraRight * position.x + cameraUp * position.y;
	gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition_worldspace, 1.0);
	
	frag_color = color;

	calculateAnimatedUV();


}

void calculateAnimatedUV()
{
	float nrOfFrames = nrAnimationFrames;

	if(nrOfFrames == 0.0)
		nrOfFrames = 1.0;

	float uv_min_x = animationIndex / nrOfFrames;
	float uv_max_x = (animationIndex + 1.0f) /nrOfFrames;

	float uv_min_y = 0.0f;
	float uv_max_y = 1.0f;

	if(uv.x <= 0.5f)
		frag_uv.x = uv_min_x;
	else
		frag_uv.x = uv_max_x;

	if(uv.y <= 0.5f)
		frag_uv.y = uv_min_y;
	else
		frag_uv.y = uv_max_y;
	
}