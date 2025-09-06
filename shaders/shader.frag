#version 450

#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform LightingBuffer {
    vec3 lightPos;
    vec3 viewPos;
    vec3 lightColor;
} light;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.lightColor;
    
    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(light.lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(light.viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64);
    vec3 specular = specularStrength * spec * light.lightColor;
    
    // Combine results
    vec3 result = (ambient + diffuse + specular); // * texture(texSampler, fragTexCoord).rgb;
    outColor = vec4(result, 1.0);
}