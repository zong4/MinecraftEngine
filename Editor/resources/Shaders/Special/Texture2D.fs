#version 330 core

// Output
out vec4 FragColor;

// Inputs
in VS_OUT
{
    vec4 Color;
    vec2 TexCoord;
    flat int TexID;
}
fs_in;

// Uniforms
uniform sampler2D u_Textures[16];

// Main
void main() { FragColor = texture(u_Textures[fs_in.TexID], fs_in.TexCoord) * fs_in.Color; }