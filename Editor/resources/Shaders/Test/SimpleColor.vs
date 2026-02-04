#version 330 core

// Layouts
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
out vec4 o_Color;

// Main
void main()
{
    // Pass through color
    o_Color = uColor;

    // Final position
    gl_Position = u_Projection * u_View * uModel * vec4(aPosition, 1.0);
}
