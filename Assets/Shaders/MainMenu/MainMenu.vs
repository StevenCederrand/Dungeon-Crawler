#version 430
layout(location=0) in vec2 position;
layout(location=1) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 frag_uv;

void main() {
    gl_Position =  vec4(position, 0, 1);
    frag_uv = uv;
}
