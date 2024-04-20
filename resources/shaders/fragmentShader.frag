#version 410

// layout(location = 0) in vec2 uvs;
out vec4 frag_colour;

// uniform sampler2D tex;

void main(){
    frag_colour = vec4(1.0, 1.0, 1.0, 1.0);//texture(tex, uvs);
}
