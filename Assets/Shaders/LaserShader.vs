#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 center;
layout(location = 2) in vec4 lengthsAndScales;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float frag_lifetime;
out float frag_initialLifetime;

void main()
{
	
	vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	
	vec3 vertexPosition_worldspace = center + cameraRight * position.x + cameraUp * position.y;
	
	vertexPosition_worldspace.x += position.x * lengthsAndScales.x * lengthsAndScales.z;
	vertexPosition_worldspace.z += -position.y * lengthsAndScales.y * lengthsAndScales.w;

	gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition_worldspace, 1.0);
	

}