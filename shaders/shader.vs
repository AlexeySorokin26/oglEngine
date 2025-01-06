#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

uniform mat4 gWorld;

out vec4 outColor;

void main(){
	gl_Position = gWorld * vec4(inPosition, 1.0);
	outColor = vec4(inColor, 1.0f);
}