#version 430

in vec2 frag_uv;

uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D colourBuffer;
uniform vec3 cameraPosition;

out vec4 finalColor;

void main() {
  finalColor = texture(colourBuffer, frag_uv);
}
