#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

in vec4 fragPosLightSpace;
uniform sampler2D shadowMap;
uniform float bias;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace) {
    float shadow = 0.0;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = texture(shadowMap, projCoords.xy).r; // calculates closest depth value from light space
    float currentDepth = projCoords.z; // calculates depth of current fragment from light's perspective

    // Determine if a fragment is in shadow or not
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
     
    return shadow;
}


void main() {
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightColor = vec3(1.0);
    vec3 objectColor = vec3(1.0);
    
    // Calculate diffuse lighting
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Calculate ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Calculate specular lighting
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalize(Normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace);
    
    // Calculate Phong Lighting
    FragColor = vec4((ambient + (1 - shadow) * (diffuse + specular)) * objectColor, 1.0);
}
