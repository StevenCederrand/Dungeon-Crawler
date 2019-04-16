#version 430

in float frag_lifetime;
in float frag_initialLifetime;

out vec4 color;

void main()
{
	color = vec4(1.0f, 0.0f, 0.0f, 1.f);
}