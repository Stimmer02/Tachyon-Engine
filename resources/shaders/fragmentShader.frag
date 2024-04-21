#version 410

in vec2 uvs;

in vec4 o_color;
out vec4 frag_colour;

uniform sampler2D tex;

void main(){
    frag_colour = /*vec4(1.0,1.0,1.0,1.0);*/texture(tex, uvs) * o_color;
}
