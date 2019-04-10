#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out GEOM_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} geom_data;


void main() {
    geom_data.position = position;
    geom_data.normal = normal;
    geom_data.uv = uv;
}
