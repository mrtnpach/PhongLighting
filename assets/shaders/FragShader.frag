#version 460 core

struct Material
{
	bool textured;
	vec3 color;
	sampler2D diffuse; // opaque type, do not instantiate struct
	sampler2D specular;
	float specularIntensity;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	float ambientIntensity;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float minRadiusCos;
	float maxRadiusCos;
};

vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDirection);
// Point lighting does depend on the relationship between the fragment's and the light's positions
vec3 CalculatePointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 CalculateSpotLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

in vec2 fTexCoord;
in vec3 fNormalVec;
in vec3 FragPos; // Use vertex WS coords to determine frag position

out vec4 FragColor;

uniform Material material;
uniform vec3 fViewPosition;

uniform DirectionalLight dirLight;
#define POINT_LIGHT_COUNT 1
uniform PointLight pointLights[POINT_LIGHT_COUNT];
//uniform PointLight pointLight;
uniform SpotLight spotLight;

void main()
{
	vec3 normal = normalize(fNormalVec);
	vec3 viewDirection = normalize(fViewPosition - FragPos);

	vec3 result = vec3(0.0, 0.0, 0.0);
	result += CalculateDirectionalLighting(dirLight, normal, viewDirection);

	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		result += CalculatePointLighting(pointLights[i], normal, FragPos, viewDirection);
	}

	result += CalculateSpotLighting(spotLight, normal, FragPos, viewDirection);

	FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);

	// Diffuse:
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// Specular:
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

	vec3 diffuseColor = material.textured ? vec3(texture(material.diffuse, fTexCoord)) : material.color;
	vec3 ambientResult = light.ambient * diffuseColor * light.ambientIntensity;
	vec3 diffuseResult = light.diffuse * diffuse * diffuseColor;
	vec3 specularResult = light.specular * specular * vec3(texture(material.specular, fTexCoord));

	return ambientResult + diffuseResult + specularResult;
}

vec3 CalculatePointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragPos); // Light direction DOES depent on frag
	// It's a vector from the light source to the frag

	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

	// Have ambient change with distance as well
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3 diffuseColor = material.textured ? vec3(texture(material.diffuse, fTexCoord)) : material.color;
	vec3 ambientResult = light.ambient * diffuseColor * 0.5;
	vec3 diffuseResult = light.diffuse * diffuse * diffuseColor;
	vec3 specularResult = light.specular * specular * vec3(texture(material.specular, fTexCoord));

	return (ambientResult + diffuseResult + specularResult) * attenuation;
}

vec3 CalculateSpotLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	// Used to be normalized vewDirection 
	vec3 lightDirection = normalize(light.position - fragPos);

	float fragRadiusCos = dot(normalize(-light.direction), lightDirection);
	float intensity = (fragRadiusCos - light.maxRadiusCos) / (light.minRadiusCos - light.maxRadiusCos);
	float clampedIntensity = clamp(intensity, 0.05, 1.0); // Light spreads and bounces

	float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

	vec3 diffuseColor = material.textured ? vec3(texture(material.diffuse, fTexCoord)) : material.color;
	vec3 ambientResult = light.ambient * diffuseColor * 0.25;
	vec3 diffuseResult = light.diffuse * diffuse * diffuseColor;
	vec3 specularResult = light.specular * specular * vec3(texture(material.specular, fTexCoord));

	return (ambientResult + (diffuseResult + specularResult) * 3) * clampedIntensity * attenuation;
	//return vec3(0.0, 1.0, 0.0);
}