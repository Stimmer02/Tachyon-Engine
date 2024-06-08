#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 normal;

out vec2 uvs;
out vec4 color;
out float cosine;

uniform mat4 u_mvp;
uniform vec4 u_color;
uniform vec4 lightPos;
uniform vec4 camPos;

void main(){

    vec4 position = vert * u_mvp;
    gl_Position = position;
    uvs = uv;

    vec4 lightVec = normalize(lightPos - position);
    vec4 viewVec = normalize(camPos - position);

    float diff = max(dot(normal, lightVec), 0.0);

    if (diff > 0.95) {
        diff = 1.0;
    } else if (diff > 0.5) {
        diff = 0.7;
    } else if (diff > 0.25) {
        diff = 0.4;
    } else {
        diff = 0.1;
    }

    color = vec4(diff, diff, diff, 1.0f);

}
