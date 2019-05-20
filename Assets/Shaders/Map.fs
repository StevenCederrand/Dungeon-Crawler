#version 430

in vec3 colour;

out vec4 finalColor;

void main()
{
	finalColor = vec4(colour, 0.2f);
}
