#version 410

layout(location = 0) in vec4 vert;
// layout(location = 1) in vec2 uv;

// out vec2 uvs;

uniform mat4 u_model;
uniform vec4 u_color;

out vec4 o_color;

void main(){
    gl_Position = vert * u_model;
    o_color = u_color;
    // uvs = uv;
}
