#version 430

layout(location = 0) out vec3 positionBuffer;
layout(location = 1) out vec3 normalBuffer;
layout(location = 2) out vec4 colourBuffer;

in vec2 frag_uv;
in vec3 frag_normal;
in vec3 frag_worldPosition;

uniform float shininess = 0.f;
uniform lowp float hasTexture = -100;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform int hasNormalMap;

void main() {
    vec4 color = texture(textureSampler, frag_uv);
    vec4 normalCol = vec4(0);
    
    if(hasNormalMap == 1) {
        normalCol = texture(normalSampler, frag_uv);
        colourBuffer.rgb = normalCol.rgb;
    }
    else {
        colourBuffer.rgb = color.rgb;
    }
    positionBuffer = frag_worldPosition;
    normalBuffer = frag_normal;
    colourBuffer.a = shininess;
}
