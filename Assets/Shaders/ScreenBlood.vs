#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

//uniform mat4 viewMatrix;
//uniform mat4 projectionMatrix;


out vec2 frag_uv;

void main()
{
	//gl_Position = projectionMatrix * viewMatrix  * vec4(position, 1.0);
	gl_Position = vec4(position, 1.0);
	frag_uv = uv;
}