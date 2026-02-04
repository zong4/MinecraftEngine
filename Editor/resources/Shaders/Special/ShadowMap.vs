#version 330 core

// Layouts
layout(location = 1) in vec3 aPos;

// Uniforms
uniform mat4 u_LightView;
uniform mat4 u_LightProjection;

// Main
void main() { gl_Position = u_LightProjection * u_LightView * vec4(aPos, 1.0); }