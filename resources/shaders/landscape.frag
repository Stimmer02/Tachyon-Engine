#version 410

in vec2 uvs;
in vec4 color;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 frag_color;

uniform sampler2D albedo;
uniform sampler2D specularMap;
uniform sampler2D aoMap;

uniform float time;
uniform vec4 lightPos;
uniform vec4 camPos;

vec2 waterDistortion(vec2 uv, float time) {
    float waveStrength = 0.1;
    float frequency = 10.0;
    float speed = 1.0;
    uv.x += sin(uv.y * frequency + time * speed) * waveStrength;
    uv.y += cos(uv.x * frequency + time * speed) * waveStrength;
    return uv;
}

void main(){
    vec2 distortedUV = waterDistortion(uvs, time);

    vec3 viewDir = normalize(camPos.xyz - fragPos);
    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    vec3 reflected = normalize(reflect(-lightDir, fragNormal));
    vec3 halfwayDir = normalize(lightDir + reflected);

    vec4 m_color = texture(albedo, distortedUV) * color;

    float specularFactor = texture(specularMap, distortedUV).r;
    float ao = texture(aoMap, distortedUV).r;

    float diff = max(dot(fragNormal, lightDir)* 0.5 + 0.5, 0.0);
    vec3 diffuse = vec3(m_color) * diff;

    float spec = pow( max(dot(fragNormal, halfwayDir), 0), specularFactor);
    vec3 specular = vec3(spec);

    vec4 finalColor = vec4(diffuse + specular, 1.0);
    frag_color = finalColor * ao;
}
