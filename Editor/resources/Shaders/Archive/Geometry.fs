#version 330 core

// Output
out vec4 FragColor;

in GS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec3 CameraPosition;
}
fs_in;

// Main
void main() { FragColor = vec4(fs_in.Normal * 0.5 + 0.5, 1.0); }
