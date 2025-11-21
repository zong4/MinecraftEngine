#version 330 core

// Layouts
layout(location = 0) in uint aEntityID;
layout(location = 1) in vec3 aPosition;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTexCoord;
layout(location = 4) in vec4 aColor;
layout(location = 5) in vec4 aMaterial;

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
    vec3 CameraPosition;

    // From Layouts
    flat uint EntityID;
    vec3 Position;
    vec3 Normal;
    vec3 TexCoord;
    vec4 Color;
    vec4 Material;
}
vs_out;

void main()
{
    vs_out.CameraPosition = u_CameraPosition;
    vs_out.EntityID = aEntityID;
    vs_out.Position = aPosition;
    vs_out.Normal = aNormal;
    vs_out.TexCoord = aTexCoord;
    vs_out.Color = aColor;
    vs_out.Material = aMaterial;
    gl_Position = u_Projection * u_View * vec4(vs_out.Position, 1.0);
}
