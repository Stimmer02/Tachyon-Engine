#version 410

in vec2 uvs;
in vec4 color;

out vec4 frag_color;
uniform sampler2D albedo;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

void main(){
    vec4 m_color = texture(albedo, uvs) * color;
    float roughness = texture(roughnessMap, uvs).r;
    float ao = texture(aoMap, uvs).r;

    frag_color = m_color * ao;
}
