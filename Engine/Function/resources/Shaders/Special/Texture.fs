#version 330 core

// Output
out vec4 FragColor;

// Inputs
in VS_OUT
{
    vec2 TexCoord;
    flat int TexID;
    vec4 Color;
}
fs_in;

uniform sampler2D u_Textures[16];

// Main
void main()
{
    // Set fragment color
    FragColor = texture(u_Textures[fs_in.TexID], fs_in.TexCoord) * fs_in.Color;
}