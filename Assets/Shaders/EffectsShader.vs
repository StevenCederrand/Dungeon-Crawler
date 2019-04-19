#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 center;
layout(location = 3) in vec4 color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 frag_color;
out vec2 frag_uv;

void main()
{
	
	vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	vec3 vertexPosition_worldspace = center + cameraRight * position.x + cameraUp * position.y;
	gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition_worldspace, 1.0);
	
	frag_color = color;
	frag_uv = uv;
}