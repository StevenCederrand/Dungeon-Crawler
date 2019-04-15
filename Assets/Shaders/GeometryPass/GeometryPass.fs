#version 430

layout(location = 0) out vec3 positionBuffer;
layout(location = 1) out vec3 normalBuffer;
layout(location = 2) out vec4 colourBuffer;

in FRAG_DATA {
    vec3 position; //in world space
    vec3 normal; //this will be the normal map
    vec2 uv; //this will be used in the fs for applying albedo
    mat3 TBN;
} frag_data;

uniform float shininess = 0.f;
uniform lowp float hasTexture = -100;

layout(binding=0) uniform sampler2D textureSampler;
layout(binding=1) uniform sampler2D normalSampler;
layout(binding=2) uniform sampler2D AOSampler;
uniform int hasAO;
uniform vec3 colorTint;


void main() {
    vec4 color = texture(textureSampler, frag_data.uv);
    vec4 aCol = vec4(0);

    vec3 normalCol = vec3(0);
    if(hasAO >= 1) {
      aCol = texture(AOSampler, frag_data.uv);
    }
    if(frag_data.TBN != mat3(0)) {
        normalCol = texture(normalSampler, frag_data.uv).rgb;
        normalCol = normalize(normalCol * 2 - 1);
        normalCol = normalize(vec3(frag_data.TBN * normalCol));
        normalBuffer = normalCol;
    } //use the default normals if there is no TBN
    else {
        normalBuffer = frag_data.normal;
    }
    colourBuffer += aCol;
    colourBuffer.rgb = mix(color.rgb, colorTint, 1.0f - colorTint.g);
    positionBuffer = frag_data.position;
    colourBuffer.a = shininess;
}
