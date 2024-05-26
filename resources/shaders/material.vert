#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 normal;

out vec2 uvs;
out vec4 color;

uniform mat4 u_mvp;
uniform sampler2D displacementMap;
uniform sampler2D normalMap;
uniform vec4 u_color;

void main(){
    float displacement = texture(displacementMap, uv).r;
    vec4 normalColor = vec4(texture(normalMap, uv).rgb, 1.0);
    vec4 normalVec = normalize(normal + (normalColor * 2.0 - 1.0));
    gl_Position = (vert + normalVec * displacement) * u_mvp;
    uvs = uv;
    color = u_color;
}
