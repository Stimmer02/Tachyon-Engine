#version 410

layout(location = 0) in vec4 vert;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 normal;

out vec2 uvs;
out vec4 color;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 u_mvp;
uniform vec4 u_color;
uniform float time;

uniform sampler2D displacementMap;
uniform sampler2D normalMap;

void main(){

    vec2 localUV = vec2(uv.x + time * 0.1f, uv.y + sin(time * 0.5) * 0.05);
    vec2 distortion = vec2(sin(localUV.y * 10.0 + time) * 0.1, cos(localUV.x * 10.0 + time) * 0.1);

    localUV += distortion * 0.5;

    float displacement = texture(displacementMap, localUV).r;
    vec4 normalColor = vec4(texture(normalMap, localUV).rgb, 1.0);
    vec4 normalVec = normalize(normal + (normalColor * 2.0 - 1.0));
    vec4 displacedVert = vert + normalVec * displacement * 0.1;
    vec4 position = displacedVert * u_mvp;

    fragNormal = normalVec.xyz;
    fragPos = displacedVert.xyz;

    gl_Position = position;
    uvs = localUV;
    color = u_color;

}
