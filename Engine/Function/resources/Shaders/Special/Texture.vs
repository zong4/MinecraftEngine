#version 330 core

// Layouts
layout(location = 1) in vec3 aPosition;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in int aTexID;
layout(location = 4) in vec4 aColor;

// Uniforms
uniform mat4 u_Model;
layout(std140) uniform UniformBuffer0
{
    // Position
    mat4 u_View;
    mat4 u_Projection;
};

// Outputs
out VS_OUT
{
    vec2 TexCoord;
    flat int TexID;
    vec4 Color;
}
vs_out;

// Main
void main()
{
    // Outputs
    vs_out.TexCoord = aTexCoord;
    vs_out.TexID = aTexID;
    vs_out.Color = aColor;

    // Final vertex position
    gl_Position = u_Projection * u_View * vec4(aPosition, 1.0);
}
