#version 330 core

// Output
out vec4 FragColor;

// Uniforms
uniform samplerCube u_Skybox;

// Inputs
in vec3 o_TexCoords;

// Main
void main() { FragColor = texture(u_Skybox, o_TexCoords); }
