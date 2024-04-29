#version 410

in vec2 uvs;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    frag_color = texture(tex, uvs);
}
