#version 430

in vec2 frag_uv;
out vec4 finalColor;

layout (location = 0) uniform sampler2D positionBuffer;
layout (location = 1) uniform sampler2D normalBuffer;
layout (location = 2) uniform sampler2D colourBuffer;

uniform vec3 sunColor;
uniform vec3 sunPosition;
uniform vec3 cameraPosition;

vec3 worldPosition;
vec3 normal;
vec3 textureColor;

layout (std140, binding = 0) uniform Lights
{
	vec4 position;
	vec4 color;
	float radius;
};

vec3 getAmbientColor(float ambientFactor)
{
	vec3 ambientColor = vec3(0.8f);
	return ambientColor * ambientFactor;
}

vec3 getDiffuseColor()
{
	vec3 toLight = normalize(sunPosition - worldPosition);
	float diffuseFactor = dot(toLight, normalize(normal));
	diffuseFactor = max(diffuseFactor, 0.f);

	return sunColor * diffuseFactor;
}

vec3 getPhongColor(float specularStrength, vec3 lightColor)
{
	vec3 toLight = normalize(sunPosition - worldPosition);
	vec3 viewDirection = normalize(cameraPosition - worldPosition);
	vec3 reflectDirection = reflect(-toLight, normalize(normal));

	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);

	return lightColor * spec * specularStrength;

}

void main() {
  
	worldPosition = texture(positionBuffer, frag_uv).rgb;
	normal = texture(normalBuffer, frag_uv).rgb;
	textureColor = texture(colourBuffer, frag_uv).rgb;
	float shininess = texture(colourBuffer, frag_uv).a;

	vec3 currentColor = getAmbientColor(0.2f) + getDiffuseColor() + getPhongColor(shininess, sunColor);

	// Clamp
	currentColor = min(currentColor, vec3(1.f));

  	finalColor = textureColor * vec4(currentColor, 1.0f);

}
