#version 430

in vec2 frag_uv;
out vec4 finalColor;

layout (binding = 0) uniform sampler2D positionBuffer;
layout (binding = 1) uniform sampler2D normalBuffer;
layout (binding = 2) uniform sampler2D colourBuffer;

uniform vec3 sunColor;
uniform vec3 sunPosition;
uniform vec3 cameraPosition;

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

vec3 getAmbientColor(float ambientFactor)
{
	vec3 ambientColor = vec3(0.8f);
	return ambientColor * ambientFactor;
}

vec3 getDiffuseColor(vec3 lightPosition, vec3 lightColor)
{
	vec3 toLight = normalize(lightPosition - worldPosition);
	float diffuseFactor = dot(toLight, normalize(normal));
	diffuseFactor = max(diffuseFactor, 0.f);

	return lightColor * diffuseFactor;
}

vec3 getPhongColor(vec3 lightPosition, float specularStrength, vec3 lightColor)
{
	vec3 toLight = normalize(lightPosition - worldPosition);
	vec3 viewDirection = normalize(cameraPosition - worldPosition);
	vec3 reflectDirection = reflect(-toLight, normalize(normal));

	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);

	return lightColor * spec * specularStrength;

}

vec3 getSumOfAllColorFromPointLights(float specularStrength, vec3 worldPosition)
{
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

void main() {

	worldPosition = texture(positionBuffer, frag_uv).rgb;
	normal = texture(normalBuffer, frag_uv).rgb;
	textureColor = texture(colourBuffer, frag_uv).rgb;
	float shininess = texture(colourBuffer, frag_uv).a;

	vec3 currentColor = getAmbientColor(0.2f)+ getDiffuseColor(sunPosition, sunColor)
	+ getPhongColor(sunPosition, shininess, sunColor) + getSumOfAllColorFromPointLights(shininess, worldPosition);
	/*

	*/

	// Clamp
	currentColor = min(currentColor, vec3(1.f));

  	finalColor = vec4(textureColor, 1.0f) * vec4(currentColor, 1.0f);

}
