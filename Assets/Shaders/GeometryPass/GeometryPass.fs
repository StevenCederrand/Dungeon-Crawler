#version 430

layout(location = 0) out vec3 positionBuffer;
layout(location = 1) out vec4 normalBuffer;
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
uniform vec3 cameraPosition;
uniform vec3 colorTint;

vec2 parallaxMapping(vec2 textureCoords, vec3 cameraPosition);

void main() {
    //Ambient Color
    vec4 aCol = vec4(0);
    //Basic normal Color
    vec3 normalCol = frag_data.normal;
    vec2 UVs = frag_data.uv;
    //If we have a normal map, then we assume as well that we have a
    //parallax occlusion map
    if(frag_data.TBN != mat3(0)) {
        mat3 TBN = transpose(frag_data.TBN);
        //Parallax Mapping
        //aCol = texture(AOSampler, frag_data.uv);
        vec3 viewDirection = normalize((TBN * cameraPosition) - (TBN * frag_data.position));
        
		//Normal Mapping
        normalCol = texture(normalSampler, UVs).rgb;
        normalCol = normalize(normalCol * 2 - 1);
        normalCol = normalize(vec3(frag_data.TBN * normalCol));

        UVs = parallaxMapping(UVs, viewDirection);
		
        if(UVs.x > 1.0 || UVs.y > 1.0 || UVs.x < 0.0 || UVs.y < 0.0)
            discard;


    }
    vec4 color = texture(textureSampler, UVs);
    normalBuffer.rgb = normalCol.rgb;
    normalBuffer.a = aCol.r;
    colourBuffer.rgb = mix(color.rgb, colorTint, 1.0f - colorTint.g);
    positionBuffer = frag_data.position;
    colourBuffer.a = shininess;
}


vec2 parallaxMapping(vec2 textureCoords, vec3 cameraPosition) {
    float h = 1.0 - texture(AOSampler, textureCoords).r; //Get the color value
    vec2 p = cameraPosition.xy * (h * 0.1); //0.5 is a height scale
    return textureCoords - p;
}
