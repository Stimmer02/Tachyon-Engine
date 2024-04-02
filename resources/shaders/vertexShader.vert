#version 410

layout(location = 0) in vec3 position;

uniform mat4 model;

void main(){
    gl_Position = vec4(position, 1.0) * model;
}
