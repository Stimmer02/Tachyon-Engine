#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 normal;

out vec2 uvs;
out vec4 color;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform vec4 u_color;

void main(){
    gl_Position = vert * u_model * u_view * u_projection;
    uvs = uv;
    color = u_color;
}
