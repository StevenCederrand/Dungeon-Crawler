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
uniform int hasNormalMap;

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
        vertices[i] = vec4(geom_data[i].position, 1);
    }

    vec4 edge1 = vertices[1] - vertices[0];
    vec4 edge2 = vertices[2] - vertices[0];
    normal = normalize(cross(vec3(edge1), vec3(edge2)));
    return normal;
}

mat3 TBN() {
    if(hasNormalMap <= 0) {
        return mat3(0);
    }

    vec3 positions[3];
	vec2 uvs[3];

	for(int i = 0; i < 3; i++) {
		uvs[i] = geom_data[i].uv;
		positions[i] = geom_data[i].position;
	}

	//Triangle edges
	vec3 edges[2];
	edges[0] = positions[1] - positions[0];
	edges[1] = positions[2] - positions[0];

	//Delta uvs
	vec2 deltaUVs[2];
	deltaUVs[0] = uvs[1] - uvs[0];
	deltaUVs[1] = uvs[2] - uvs[0];
	//Division formula
	float formula = 1.0f / (deltaUVs[0].x * deltaUVs[1].y - deltaUVs[1].x * deltaUVs[0].y);
	//Tangent
	vec3 tangent;
	tangent.x = formula * (deltaUVs[1].y * edges[0].x - deltaUVs[0].y * edges[1].x);
	tangent.y = formula * (deltaUVs[1].y * edges[0].y - deltaUVs[0].y * edges[1].y);
	tangent.z = formula * (deltaUVs[1].y * edges[0].z - deltaUVs[0].y * edges[1].z);
	tangent = normalize(tangent);
	//Bitangent
	vec3 bitangent;
	bitangent.x = formula * (-deltaUVs[1].x * edges[0].x + deltaUVs[0].x * edges[1].x);
	bitangent.y = formula * (-deltaUVs[1].x * edges[0].y + deltaUVs[0].x * edges[1].y);
	bitangent.z = formula * (-deltaUVs[1].x * edges[0].z + deltaUVs[0].x * edges[1].z);
	bitangent = normalize(bitangent);
    //Because the light pass handles everything in world spaceS
	vec3 T = normalize(vec3(modelMatrix*vec4(tangent, 0.0)));
	vec3 B = normalize(vec3(modelMatrix*vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix*vec4(normalize(cross(edges[0], edges[1])), 0.0)));
	return mat3(T, B, N);
}


void main() {
    vec4 vertex;
    for(int i = 0; i < 3; i++) {
        vertex = projectionMatrix * viewMatrix * modelMatrix * vec4(geom_data[i].position, 1);
        gl_Position = vertex;
        frag_data.uv = geom_data[i].uv;
        frag_data.position = vec3(modelMatrix * vec4(geom_data[i].position, 1.0f));
        frag_data.TBN = TBN();
        frag_data.normal = mat3(transpose(inverse(modelMatrix))) * getNormal();
        EmitVertex();
    }
    EndPrimitive();
}
