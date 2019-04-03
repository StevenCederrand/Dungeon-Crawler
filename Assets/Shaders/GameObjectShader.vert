#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 frag_uv;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position,1.0f);
	frag_uv = uv;
}
