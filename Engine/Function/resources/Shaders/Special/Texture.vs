#version 330 core

// Layouts
layout(location = 1) in vec3 aPosition;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in int aTexID;
layout(location = 4) in vec4 aColor;

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
    vec2 TexCoord;
    flat int TexID;
    vec4 Color;
}
vs_out;

// Main
void main()
{
    vs_out.TexCoord = aTexCoord;
    vs_out.TexID = aTexID;
    vs_out.Color = aColor;

    gl_Position = u_Projection * u_View * vec4(aPosition, 1.0);
}
