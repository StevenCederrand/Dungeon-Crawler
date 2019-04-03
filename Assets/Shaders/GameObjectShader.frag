#version 430

uniform sampler2D textureSampler;

in vec2 frag_uv;
out vec4 finalColor;

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);

	finalColor = textureColor;
}