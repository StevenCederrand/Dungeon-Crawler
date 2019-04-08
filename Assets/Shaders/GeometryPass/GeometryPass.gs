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

//uniform int hasNormalMap; //Set as float if it doens't work
uniform vec3 cameraPosition;

out FRAG_DATA {
    vec3 position; //in world space
    vec3 normal; //this will be the normal map
    vec2 uv; //this will be used in the fs for applying albedo
    //mat3 TBN;
    //int hasNormalMap;
} frag_data;

vec3 getNormal() {
    vec3 normal = vec3(0, 0, 0);
    vec4 vertices[3];
    for(int i = 0; i < 3; i++) {
        vertices[i] = modelMatrix * vec4(geom_data[i].position, 1);
    }

    vec4 edge1 = vertices[1] - vertices[0];
    vec4 edge2 = vertices[2] - vertices[0];
    normal = normalize(cross(vec3(edge1), vec3(edge2)));
    return normal;
}
/*
mat3 TBN(vec3 normal) {
    if(hasNormalMap <= 0) {
        return Mat3(0);
    }


    return Mat3(1);
}*/
void backfaceCulling() {
    vec4 vertex;
    bool drawTriangle = true;
    for(int i = 0; i < 3 && drawTriangle; i++) {
        vertex = projectionMatrix * viewMatrix * modelMatrix * vec4(geom_data[i].position, 1);
        vec3 cameraToSurface = normalize(cameraPosition - vec3(vertex));
        if(dot(cameraToSurface, getNormal()) <= 0.0f) {
            drawTriangle = false;
        }
        else {
            gl_Position = vertex;
            frag_data.uv = geom_data[i].uv;
            frag_data.position = vec3(modelMatrix * vec4(geom_data[i].position, 1.0f));
            frag_data.normal = geom_data[i].normal; // mat3(transpose(inverse(modelMatrix))) *
            EmitVertex();
        }
    }
    if(drawTriangle) {
        EndPrimitive();
    }
}



void main() {
    //this will also apply the normal TBN
    //backfaceCulling();
    vec4 vertex;
    for(int i = 0; i < 3; i++) {
        vertex = projectionMatrix * viewMatrix * modelMatrix * vec4(geom_data[i].position, 1);
        gl_Position = vertex;
        frag_data.uv = geom_data[i].uv;
        frag_data.position = vec3(modelMatrix * vec4(geom_data[i].position, 1.0f));
        frag_data.normal = geom_data[i].normal; // mat3(transpose(inverse(modelMatrix))) *
        EmitVertex();
    }
    EndPrimitive();


}
