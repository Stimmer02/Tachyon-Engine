#version 410

in vec2 uvs;

out vec4 frag_colour;

uniform sampler2D tex;

void main(){
    vec4 tex_color = texture(tex, uvs);
    frag_colour = texture(tex, uvs);
}
