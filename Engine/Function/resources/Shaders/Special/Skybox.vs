#version 330 core

// Layouts
layout(location = 0) in vec3 aPosition;

// Uniforms
layout(std140) uniform UniformBuffer0
{
    mat4 u_View;
    mat4 u_Projection;
};

// Outputs
out vec3 o_TexCoords;

// Main
void main()
{
    o_TexCoords = aPosition;
    vec4 pos = u_Projection * mat4(mat3(u_View)) * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}
