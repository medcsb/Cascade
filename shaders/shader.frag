#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;


layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 inverseviewMatrix;
    vec4 ambientLightColor;
    vec4 lightColor;
    vec3 lightPosition;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec3 directionToLight = ubo.lightPosition - fragPosWorld;
    float distance2 = dot(directionToLight, directionToLight);
    float attenuation = 1.0 / distance2; // 1/d^2 falloff

    vec3 surfaceNormal = normalize(fragNormalWorld);
    vec3 lightDir = normalize(directionToLight);

    vec3 cameraPosWorld = ubo.inverseviewMatrix[3].xyz;
    vec3 viewDir = normalize(cameraPosWorld - fragPosWorld);

    // Light intensities
    vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
    vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 diffuseLight = lightColor * max(dot(surfaceNormal, lightDir), 0.0);

    // --- Specular ---
    vec3 reflectDir = reflect(-lightDir, surfaceNormal); // reflection direction
    float specStrength = 1.5;       // adjust as needed
    float shininess = 32.0;         // higher = smaller, sharper specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularLight = specStrength * spec * lightColor;

    // Final color
    vec3 finalColor = (ambientLight + diffuseLight + specularLight) * fragColor;
    outColor = vec4(finalColor, 1.0);
}