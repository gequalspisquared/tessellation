#version 460 core

in float height;

out vec4 frag_color;

uniform float scale;
uniform float shift;

void main() {
    float intensity = clamp((height+shift) / scale / 255, 0.0f, 1.0f);
    frag_color = vec4(intensity, intensity, intensity, 1.0f);
}