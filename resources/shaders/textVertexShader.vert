#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;

out vec2 uvs;

uniform mat4 u_projection;
uniform vec4 u_translation;

void main(){
    gl_Position = (vert + u_translation) * u_projection;
    uvs = uv;
}
