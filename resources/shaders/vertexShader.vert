#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 col;

out vec2 uvs;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec4 o_color;

void main(){
    gl_Position = u_model *  vert;
    uvs = uv;
    o_color = col;
}
