#version 410

in vec2 uvs;
out vec4 frag_colour;

uniform sampler2D tex;

void main(){
    frag_colour = texture(tex, uvs);
}
