#version 430

in vec2 frag_uv;
out vec4 finalColor;

layout (binding = 0) uniform sampler2D positionBuffer;
layout (binding = 1) uniform sampler2D normalBuffer;
layout (binding = 2) uniform sampler2D colourBuffer;
layout (binding = 3) uniform sampler2D shadowBuffer;

uniform struct Spotlight {
	vec3 position;
	vec3 direction;
	float radius;
} spotlight;

uniform vec4 flashColor;
uniform vec4 flashPosition;

uniform vec3 sunColor;
uniform vec3 sunPosition;
uniform vec3 cameraPosition;

uniform mat4 lightSpaceMatrix;

vec3 worldPosition;
vec3 normal;
vec3 textureColor;

uniform int numberOfLights;

struct lightData
{
	vec4 position;
	vec4 color;
};

layout (std140, binding = 0) buffer Lights
{
	lightData lightBuffer[];
};

vec3 getAmbientColor(float ambientFactor);
vec3 getDiffuseColor(vec3 lightPosition, vec3 lightColor);
vec3 getPhongColor(vec3 lightPosition, float specularStrength, vec3 lightColor);
vec3 getSumOfAllColorFromPointLights(float specularStrength, vec3 worldPosition);
vec3 getSumOfSpotlights(vec3 worldPosition);
float shadowCalculations(vec4 lightSpacePos);
vec3 flashEffect(vec3 worldPosition);

void main() {

	worldPosition = texture(positionBuffer, frag_uv).rgb;

	normal = texture(normalBuffer, frag_uv).rgb;
	textureColor = texture(colourBuffer, frag_uv).rgb;
	float shininess = texture(colourBuffer, frag_uv).a;

	vec4 lightSpacePos = lightSpaceMatrix * vec4(worldPosition, 1);

	float shadow = shadowCalculations(lightSpacePos);

	vec3 currentColor = vec3(0);
	// If something is not in shadow then add spotlight values
	if(shadow == 0) {
		currentColor = getSumOfSpotlights(worldPosition);
	}

	currentColor += getAmbientColor(0.2f) * (getDiffuseColor(sunPosition, sunColor)
	+ getPhongColor(sunPosition, shininess, sunColor)) + getSumOfAllColorFromPointLights(shininess, worldPosition) + flashEffect(worldPosition);


	// Clamp
	currentColor = min(currentColor, vec3(1.f));

  	finalColor = vec4(textureColor, 1.0f) * vec4(currentColor, 1.0f);
}

//Calculate the shadow map
float shadowCalculations(vec4 lightSpacePos) {
	//Perspective divide
	vec3 projectCoords = lightSpacePos.xyz / lightSpacePos.w;
	projectCoords = projectCoords * 0.5 + 0.5;

	//If we're sampling outside of the texture, just return 1
	if(projectCoords.z >= 1.0) {
		return 1.0;
	}
	//Avoid sampling outside of the texture
	if(projectCoords.x < 0 || projectCoords.x > 1) {
		return 1.0f;
	}
	if(projectCoords.y < 0 || projectCoords.y > 1) {
		return 1.0f;
	}
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(shadowBuffer, 0);

	//Sample the depthmap and get the closest depth from the light
	float closestDepth = texture(shadowBuffer, projectCoords.xy).r;
	float currentDepth = projectCoords.z;
	float bias = 0.0001f;

	for(int i = -1; i <= 1; ++i) {
		for(int j = -1; j <= 1; ++j) {
			float pcfDepth = texture(shadowBuffer, projectCoords.xy + vec2(i, j) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0f;

	return shadow;
}

vec3 flashEffect(vec3 worldPosition) {

	if(flashColor.w == 0) {
		return vec3(0);
	}
	vec3 lightPosition = vec3(flashPosition);
	float dist = length(lightPosition - worldPosition);
	if(dist >= flashColor.w) {
		return vec3(0);
	}

	vec3 colour = getDiffuseColor(lightPosition, vec3(flashColor)) + getPhongColor(lightPosition, 0.1f, vec3(flashColor));
	float strength = clamp((flashColor.w - dist) / flashColor.w, 0.f, 1.0f);
	return colour * strength;
}

//Calculate the spotlight
vec3 getSumOfSpotlights(vec3 worldPosition) {
	vec3 lightDirection = normalize(spotlight.position - worldPosition);
	float radialVal = dot(lightDirection, normalize(-spotlight.direction));

	if(radialVal >= spotlight.radius) {
		//Do Something
		return vec3(0.2f);
	}
	else {
		return vec3(0);
	}
}
//Calculate all of the pointlight stuff
vec3 getSumOfAllColorFromPointLights(float specularStrength, vec3 worldPosition) {
	vec3 finalColor = vec3(0.f);
	for(int i = 0; i < numberOfLights; i++)
	{
		vec3 lightPosition = lightBuffer[i].position.xyz;
		vec3 lightColor =  lightBuffer[i].color.rgb;
		float lightRadius =  lightBuffer[i].color.a;
		float dist = length(lightPosition - worldPosition);

		if(dist >= lightRadius)
			continue;

		vec3 currentColor = getDiffuseColor(lightPosition, lightColor) + getPhongColor(lightPosition, specularStrength, lightColor);
		float strength = clamp((lightRadius - dist) / lightRadius, 0.f, 1.0f);
		finalColor += currentColor * strength;
	}

	return finalColor;
}

vec3 getPhongColor(vec3 lightPosition, float specularStrength, vec3 lightColor) {
	vec3 toLight = normalize(lightPosition - worldPosition);
	vec3 viewDirection = normalize(cameraPosition - worldPosition);
	vec3 reflectDirection = reflect(-toLight, normalize(normal));

	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);

	return lightColor * spec * specularStrength;

}

vec3 getDiffuseColor(vec3 lightPosition, vec3 lightColor)
{
	vec3 toLight = normalize(lightPosition - worldPosition);
	float diffuseFactor = dot(toLight, normalize(normal));
	diffuseFactor = max(diffuseFactor, 0.f);

	return lightColor * diffuseFactor;
}

vec3 getAmbientColor(float ambientFactor)
{
	vec3 ambientColor = vec3(0.8f);
	return ambientColor * ambientFactor;
}

/*
uniform struct Spotlight {
	vec3 position;
	vec3 direction;
	float radius;
} spotlight;*/
