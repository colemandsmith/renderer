#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 fragPos;
in vec4 directionalLightSpacePos;
in mat3 TBN;

out vec4 colour;

vec3 mappedNormal;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight {
	Light base;
	vec3 direction;
};

struct PointLight { 
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight {
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap {
	samplerCube shadowMap;
	float farPlane;
};

struct Material {
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform sampler2D normalMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

vec3 gridSamplingDisc[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcDirectionalShadowFactor(DirectionalLight light) {
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	// scaling a -1 to 1 range to a 0 to 1 range
	projCoords = (projCoords * 0.5) + 0.5;
	
	// orthogonal projection == no consideration of z coords
	float closest = texture(directionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;
	
	vec3 norm = normalize(mappedNormal);
	vec3 lightDir = normalize(light.direction);
	
	float bias = max(0.05 * (1.0 - dot(mappedNormal, lightDir)), 0.005);
	
	// PCF to smooth out shadow
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			
			// 1.0 == full shadow;
			// if current point is farther than the closest depth, then the point is in shadow
			shadow += current - bias > closest ? 1.0 : 0.0;
		}
	}
	
	// div by number of sampled pixels
	shadow /= 9;
	
	if (projCoords.z > 1.0) {
		shadow = 0.0;
	}
	
	return shadow;
}

float CalcOmniShadowFactor(PointLight pLight, int shadowIndex) {
	vec3 fragToLight = fragPos - pLight.position;
	
	float current = length(fragToLight);
	
	float shadow = 0.0;
	float bias = 0.05;
	int totalSamples = 20;
	
	float viewDistance = length(eyePosition - fragPos);
	// scale based on view distance
	float discRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;
	
	for (int i = 0; i < totalSamples; i++) {
		float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + gridSamplingDisc[i] * discRadius).r;
		
		// scale from range to real distance
		closest *= omniShadowMaps[shadowIndex].farPlane;
		
		if (current - bias > closest) {
			shadow += 1.0;
		}
	}
	shadow /= float(totalSamples);
	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) {
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	float diffuseFactor = max(dot(normalize(mappedNormal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f) ;
	
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if (diffuseFactor > 0.0f) {
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(mappedNormal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f) {
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight() {
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex) {
    vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);
	
	vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);
	
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex) {
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);
	
	if (slFactor > sLight.edge) {
		vec4 color = CalcPointLight(sLight.base, shadowIndex);
		// Use the slfactor as a factor on the result, to soften the edges
		// Scale the value to between 0 and 1 to make the effect consistent
		return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
	} 
	else {
		return vec4(0, 0.0, 0, 0);
	}
}

vec4 CalcPointLights() {
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; i++) {
		totalColor += CalcPointLight(pointLights[i], i);
	}
	
	return totalColor;
}

vec4 CalcSpotLights() {
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < spotLightCount; i++) {
		totalColor += CalcSpotLight(spotLights[i], i + pointLightCount);
	}
	
	return totalColor;
}

void main() {
	mappedNormal = texture(normalMap, texCoord).rgb;
	
	// convert to range -1, 1
	mappedNormal = mappedNormal * 2.0 - 1.0;
	mappedNormal = normalize(TBN * mappedNormal);
	
	
	vec4 finalColor = CalcDirectionalLight();
	//finalColor += CalcPointLights();
	//finalColor += CalcSpotLights();
    colour = texture(theTexture, texCoord) * finalColor;
}