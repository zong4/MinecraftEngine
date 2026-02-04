#version 330 core

// Layouts
layout(location = 1) in vec3 aPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in int aTexID;

// Uniforms
layout(std140) uniform UniformBuffer0
{
    mat4 u_View;
    mat4 u_Projection;
    vec3 u_CameraPosition;
};

// Outputs
out VS_OUT
{
    vec4 Color;
    vec2 TexCoord;
    flat int TexID;
}
vs_out;

// Main
void main()
{
    // Pass through attributes
    vs_out.Color = aColor;
    vs_out.TexCoord = aTexCoord;
    vs_out.TexID = aTexID;

    // Final position
    gl_Position = u_Projection * u_View * vec4(aPosition, 1.0);
}
