#version 430		

in vec2 frag_uv;
in vec3 frag_normal;
in vec3 frag_worldPosition;

out vec4 finalColor;

uniform sampler2D textureSampler;
uniform vec3 sunColor;
uniform vec3 sunPosition;
uniform vec3 cameraPosition;
uniform float shininess = 1.f;

vec3 getAmbientColor(float ambientFactor)
{
	vec3 ambientColor = vec3(0.8f);
	return ambientColor * ambientFactor;
}

vec3 getDiffuseColor()
{
	vec3 toLight = normalize(sunPosition - frag_worldPosition);
	float diffuseFactor = dot(toLight, normalize(frag_normal));
	diffuseFactor = max(diffuseFactor, 0.f);

	return sunColor * diffuseFactor;
}

vec3 getPhongColor(float specularStrength, vec3 lightColor)
{
	vec3 toLight = normalize(sunPosition - frag_worldPosition);
	vec3 viewDirection = normalize(cameraPosition - frag_worldPosition);
	vec3 reflectDirection = reflect(-toLight, normalize(frag_normal));

	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);

	return lightColor * spec * specularStrength;

}

void main()
{	
	vec4 textureColor = texture(textureSampler, frag_uv);

	vec3 currentColor = getAmbientColor(0.2f) + getDiffuseColor() + getPhongColor(shininess, sunColor);

	currentColor = min(currentColor, vec3(1.f));


	finalColor = textureColor * vec4(currentColor, 1.0f);
}