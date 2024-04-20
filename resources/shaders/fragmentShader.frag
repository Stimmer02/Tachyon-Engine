#version 410

// layout(location = 0) in vec2 uvs;
in vec4 o_color;
out vec4 frag_colour;

// uniform sampler2D tex;

void main(){
    frag_colour = o_color;//texture(tex, uvs);
}
