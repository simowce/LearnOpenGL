#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;
// mat4 transform = mat4(0.0, 0.5, 0.0, 0.0,
//                  -0.5, 0.0, 0.0, 0.0,
//                   0.0, 0.0, 0.5, 0.0,
//                   0.0, 0.0, 0.0, 1.0);

void main() {
    // gl_Position = vec4(aPos, 1.f);
    // TexCoord = aTexCoord;
    gl_Position = transform * vec4(aPos, 1.f);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}