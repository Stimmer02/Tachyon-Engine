#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

out vec2 uvs;

uniform mat4 model;
uniform vec3 offset;

void main(){
    gl_Position = vec4(position + offset, 1.0) * model;
    uvs = uv;
}
