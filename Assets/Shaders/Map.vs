#version 430

layout(location = 0) in vec3 position;

out vec3 colour;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform int	roomHasPlayer;
void main()
{
	colour = vec3(0.0f, 1.0f, 0.0f);
	if(roomHasPlayer == 1)
		colour = vec3(1.0f, 0.0f, 0.0f);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}