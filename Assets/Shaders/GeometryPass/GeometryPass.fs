#version 430

layout(location = 0) out vec3 positionBuffer;
layout(location = 1) out vec3 normalBuffer;
layout(location = 2) out vec4 colourBuffer;

in FRAG_DATA {
    vec3 position; //in world space
    vec3 normal; //this will be the normal map
    vec2 uv; //this will be used in the fs for applying albedo
} frag_data;

uniform float shininess = 0.f;
uniform lowp float hasTexture = -100;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform int hasNormalMap;

void main() {
    vec4 color = texture(textureSampler, frag_data.uv);
    vec4 normalCol = vec4(0);

    if(hasNormalMap == 1) {
        normalCol = texture(normalSampler, frag_data.uv);
        colourBuffer.rgb = normalCol.rgb;
    }
    else {
        colourBuffer.rgb = color.rgb;
    }
    positionBuffer = frag_data.position;
    normalBuffer = frag_data.normal;
    colourBuffer.a = shininess;
}
