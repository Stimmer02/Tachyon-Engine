#version 410

in vec2 uvs;
in vec4 color;
in float cosine;

out vec4 frag_color;
uniform sampler2D albedo;


void main(){
    vec4 m_color = texture(albedo, uvs) * color;
    frag_color = m_color;
}
