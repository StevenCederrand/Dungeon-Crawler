#version 430

uniform sampler2D textureSampler;

in vec2 frag_uv;
out vec4 finalColor;

layout (std140, binding = 0) uniform Lights
{
	vec4 position;
	vec4 color;
	float radius;
};

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);

	finalColor = textureColor * color;
}