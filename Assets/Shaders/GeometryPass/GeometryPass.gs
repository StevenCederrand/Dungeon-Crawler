#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in GEOM_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} geom_data[];

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out FRAG_DATA {
    vec3 position; //in world space
    vec3 normal; //this will be the normal map
    vec2 uv; //this will be used in the fs for applying albedo
    mat3 TBN;
} frag_data;

vec3 getNormal() {
    vec3 normal = vec3(0, 0, 0);
    vec4 vertices[3];
    for(int i = 0; i < 3; i++) {
        vertices[i] = projectionMatrix * viewMatrix * modelMatrix * vec4(geom_data[i].position, 1);
    }

    vec3 edge1 = vertices[1] - vertices[0];
    vec3 edge2 = vertices[2] - vertices[0];
    normal = normalize(cross(edge1, edge2));

    return normal;
}

void backfaceCull() {

}

void normalMap() {

}

void main() {
    //this will also apply the normal TBN

}
