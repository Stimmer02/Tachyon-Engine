#version 410

layout(location = 0) in vec4 vert;
// layout(location = 1) in vec2 uv;

// out vec2 uvs;

uniform vec4 position;
uniform mat4 model;
// uniform vec4 offset;

void main(){
    gl_Position = (vert + position) * model;
    // uvs = uv;
}
