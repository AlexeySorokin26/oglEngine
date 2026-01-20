#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 finalMatrix;

void main() {
    gl_Position = finalMatrix * vec4(position, 1.0);
}
