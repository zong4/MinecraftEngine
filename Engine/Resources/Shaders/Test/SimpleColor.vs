#version 330 core

layout(location = 0) in vec3 aPosition;

// Uniforms
layout(std140) uniform UniformBuffer0
{
    mat4 u_View;
    mat4 u_Projection;
    vec3 u_CameraPosition;
};
uniform mat4 uModel;
uniform vec4 uColor;

// Outputs
out VS_OUT { vec4 Color; }
vs_out;

void main()
{
    vs_out.Color = uColor;
    gl_Position = u_Projection * u_View * uModel * vec4(aPosition, 1.0);
}
