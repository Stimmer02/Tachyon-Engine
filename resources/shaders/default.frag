#version 410

in vec2 uvs;
in vec4 color;

out vec4 frag_color;
uniform sampler2D tex;

void main(){
    vec4 m_color = texture(tex, uvs) * color;
    frag_color = m_color;
}
