#version 330 core

in vec4 outColor;
in vec2 outTex;

out vec4 FragColor;

uniform vec4 fragColor;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main() {
    // FragColor = mix(texture(ourTexture, outTex), fragColor, 0.2);
    // FragColor = texture(ourTexture, outTex) * outColor;
    vec2 laughCor = vec2(-outTex.s, outTex.t);
    FragColor = mix(texture(ourTexture, outTex), texture(ourTexture2, laughCor), 0.2f);
}
