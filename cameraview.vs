#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragPosLightSpace;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0)); // calculates the fragment position from the camera space
    Normal = mat3(transpose(inverse(model))) * aNormal; // calculates the normal of each fragment
    fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); // calculates the fragment position from the light space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
