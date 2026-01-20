#version 330 core

// Layouts
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aVelocity;
layout(location = 2) in float aLife;
layout(location = 3) in float aLifeMax;

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
    float Life;
    float LifeMax;
}
vs_out;

// Main
void main()
{
    vs_out.Life = aLife;
    vs_out.LifeMax = aLifeMax;

    gl_Position = u_Projection * u_View * vec4(aPosition, 1.0);
}