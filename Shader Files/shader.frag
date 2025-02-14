#version 460 core

uniform vec3 planetColor;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform bool useLighting;

in vec3 interpolatedNormal;
in vec3 fragPos;

out vec4 FragColor;
void main()
{
    if (!useLighting) {
        FragColor = vec4(planetColor, 1.0);
        return;
    }
    vec3 normal = normalize(interpolatedNormal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float angle = max(dot(normal, lightDir), 0.0);
    FragColor = vec4(angle * lightColor * planetColor, 1.0);
}