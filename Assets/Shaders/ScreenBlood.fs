#version 430

in vec2 frag_uv;

uniform sampler2D textureSampler;
out vec4 color;
uniform float alpha;

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);
	textureColor.a = textureColor.a * alpha;
	color = textureColor;
}