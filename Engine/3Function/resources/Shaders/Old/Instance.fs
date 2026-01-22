#version 330 core

// Output
out vec4 FragColor;

// Uniforms
uniform samplerCube u_Skybox;

// Inputs
in VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec3 CameraPosition;
}
fs_in;

// Main
void main() { FragColor = vec4(fs_in.Position, 1.0); }
