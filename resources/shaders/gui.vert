#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;

out vec2 uvs;
out vec4 color;

uniform mat4 u_projection;
uniform mat4 u_model;

void main(){
    gl_Position = vert * u_model * u_projection ;
    uvs = uv;
    color = vec4(1.0);
}
