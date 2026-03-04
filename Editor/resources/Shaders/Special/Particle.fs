#version 330 core

// Output
out vec4 FragColor;

// Inputs
in VS_OUT
{
    float Life;
    float LifeMax;
}
vs_in;

// Uniforms
uniform vec4 u_Color;

// Main
void main() { FragColor = u_Color; }