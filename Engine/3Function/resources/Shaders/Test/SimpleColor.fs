#version 330 core

// Output
out vec4 FragColor;

// Inputs
in VS_OUT { vec4 Color; }
fs_in;

// Main
void main() { FragColor = fs_in.Color; }