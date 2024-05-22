#version 410

in vec2 uvs;
in vec4 color;

const mat3 ditherMatrix = mat3(
    vec3(0.25, 0.75, 0.25),
    vec3(0.75, 1.0, 0.75),
    vec3(0.25, 0.75, 0.25)
);

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    vec4 m_color = texture(tex, uvs) * color;
    float ditheredValue = ditherMatrix[int(gl_FragCoord.x) % 3][int(gl_FragCoord.y) % 3];
    frag_color = vec4(vec3(ditheredValue), 1.0) * m_color;
}
