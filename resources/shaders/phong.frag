#version 410

in vec2 uvs;
in vec4 color;
in vec4 fragPos;
in vec4 fragNormal;

out vec4 frag_color;

uniform vec4 lightPos;
uniform vec4 camPos;

const float metallicFactor = 0.5;
const float smoothness = 0.9;

void main(){

    vec3 lightDir = normalize(lightPos.xyz - fragPos.xyz);
    vec3 viewDir = normalize(camPos.xyz - fragPos.xyz);
    vec3 normal = normalize(fragNormal.xyz);

    float diff = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = reflect(-viewDir, normal);

    float specular = pow(max(dot(reflectDir, lightDir), 0.0), 64.0);

    vec4 colorMix = mix(color, vec4(1.0), specular * metallicFactor);

    vec4 finalColor = mix(color, colorMix, smoothness);

    frag_color = finalColor * diff;
}
