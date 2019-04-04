#version 430

layout(location = 0) out vec3 positionBuffer;
layout(location = 1) out vec3 normalBuffer;
layout(location = 2) out vec4 colourBuffer;

in vec2 frag_uv;
in vec3 frag_normal;
in vec3 frag_worldPosition;

uniform float shininess = 0.f;
uniform sampler2D textureSampler;

void main() {
  vec4 color = texture(textureSampler, frag_uv);
  
  positionBuffer = frag_worldPosition;
  normalBuffer = frag_normal;
  colourBuffer.rgb = color.rgb;
  colourBuffer.a = shininess;
}
