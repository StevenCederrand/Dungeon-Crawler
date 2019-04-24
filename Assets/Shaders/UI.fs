#version 430

in vec2 frag_uv;

uniform sampler2D textureSampler;
out vec4 color;

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);
	color = textureColor;
}