#version 460 core

layout (location = 0) in vec3 position;

out float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    height = position.y;
    gl_Position = projection * view * model * vec4(position.xyz, 1.0);
}