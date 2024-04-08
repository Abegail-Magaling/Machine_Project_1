#version 330 core

out vec4 FragColor; // Returns a color

uniform sampler2D tex0;

in vec2 texCoord;

in vec3 normCoord;

in vec3 fragPos;

uniform vec3 lightPos; //position of light

uniform vec3 lightColor; //color of light

uniform float ambientStr;

uniform vec3 ambientColor;

uniform vec3 cameraPos;

uniform float specStr;

uniform float specPhong;

uniform float brightness; //added parameter for brightness

uniform vec3 direction;

uniform vec3 dirLightColor;

uniform vec3 dirAmbientColor;

uniform float dirAmbientStr;

uniform float dirSpecStr;

uniform float dirSpecPhong;

uniform float dirIntensity;


//Simple shader that colors the model 
void main()
{
	vec3 normal = normalize(normCoord);
	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0); //apply diffuse formula
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor; //add brightness

	float distance = distance(lightPos, fragPos);
	float brightness = brightness * (1.0 / (distance * distance));

	vec3 pointLightResult = vec3(specColor + diffuse + ambientCol) * brightness;

	//				  R   G   B  a  Ranges from 0->1
	//FragColor = vec4(0.60f, 0.40f, 1.0f, 1.0f); //Sets the color of the fragment

	//FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * texture(tex0, texCoord);


	//Calculate Directional Light
	vec3 dirNormal = normalize(normCoord);
	vec3 dirLightDir = direction;
	
	float dirDiff = max(dot(dirNormal, dirLightDir), 0.0);
	vec3 dirDiffuse = dirDiff * dirLightColor;

	vec3 dirAmbientCol = dirAmbientColor * dirAmbientStr;

	vec3 dirViewDir = normalize(cameraPos - fragPos);
	vec3 dirReflectDir = reflect(-dirLightDir, dirNormal);

	float dirSpec = pow(max(dot(dirReflectDir, dirViewDir), 0.1), dirSpecPhong);
	vec3 dirSpecColor = dirSpec * dirSpecStr * dirLightColor;

	float dirIntensity = dirIntensity;

	vec3 directionalLightResult = vec3(dirSpecColor + dirDiffuse + dirAmbientCol) * dirIntensity;

	FragColor = vec4(pointLightResult + directionalLightResult, 1.0) * texture(tex0, texCoord);
}