#version 330 core

// Output
out uvec4 FragColor;

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
    flat int TexID;
}
fs_in;

// Main
void main() { FragColor = uvec4(fs_in.EntityID, 0, 0, 1); }
