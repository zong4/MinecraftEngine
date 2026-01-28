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

// Main
void main() { FragColor = vec4(0.0, 1.0, 0.0, 1.0); }