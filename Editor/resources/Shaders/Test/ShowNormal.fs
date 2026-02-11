#version 330 core

// Output
out vec4 FragColor;

// Inputs
in VS_OUT
{
    vec3 CameraPosition;

    // From Layouts
    flat uint EntityID;
    vec3 Position;
    vec3 Normal;
    vec4 Material;
    vec4 Color;
    vec3 TexCoord;
}
fs_in;

// Main
void main() { FragColor = vec4(fs_in.Normal, 1.0); }
