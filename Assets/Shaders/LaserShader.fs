#version 430

in vec4 frag_color;
in vec2 frag_uv;

uniform sampler2D textureSampler;
out vec4 color;

void main()
{
	vec4 textureColor = texture(textureSampler, frag_uv);
	
	if(textureColor.a < 0.5)
		discard;
	
	color = frag_color;
}