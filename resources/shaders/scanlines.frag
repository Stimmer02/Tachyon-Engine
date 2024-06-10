#version 410

in vec2 uvs;
in vec4 color;

out vec4 frag_color;
uniform sampler2D albedo;

void main(){
    vec4 baseColor = texture(albedo, uvs) * color;

    float scanline = sin(gl_FragCoord.y * 30.0) * 0.1;

    float barrelDistortion = 0.02;
    vec2 centered_coord = (gl_FragCoord.xy / vec2(800.0, 600.0)) * 2.0 - 1.0;
    float r2 = dot(centered_coord, centered_coord);
    vec2 distorted_coord = centered_coord * (1.0 + barrelDistortion * r2);

    distorted_coord = (distorted_coord + 1.0) * 0.5 * vec2(800.0, 600.0);

    float chromaOffset = 0.003;

    vec2 chromaR = distorted_coord + vec2(chromaOffset, 0.0);
    vec2 chromaB = distorted_coord - vec2(chromaOffset, 0.0);

    vec4 texColorR = texture(albedo, uvs + (chromaR - gl_FragCoord.xy) / vec2(800.0, 600.0)) * color;
    vec4 texColorB = texture(albedo, uvs + (chromaB - gl_FragCoord.xy) / vec2(800.0, 600.0)) * color;

    vec4 finalColor = vec4(texColorR.r, baseColor.g, texColorB.b, baseColor.a);

    finalColor.rgb *= (1.0 - scanline);

    frag_color = finalColor;
}
