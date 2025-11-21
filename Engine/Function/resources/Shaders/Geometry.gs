#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT
{
    vec3 CameraPosition;

    flat uint EntityID;
    vec3 Position;
    vec3 Normal;
    vec3 TexCoord;
    vec4 Color;
    vec4 Material;
}
gs_in[];

out GS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec3 CameraPosition;
}
gs_out;

// vec4 explode(vec4 position, vec3 normal);

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main()
{
    // gl_Position = explode(gl_in[0].gl_Position, GetNormal());
    gl_Position = gl_in[0].gl_Position;
    gs_out.Position = gs_in[0].Position;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.CameraPosition = gs_in[0].CameraPosition;
    EmitVertex();

    // gl_Position = explode(gl_in[1].gl_Position, GetNormal());
    gl_Position = gl_in[1].gl_Position;
    gs_out.Position = gs_in[1].Position;
    gs_out.Normal = gs_in[1].Normal;
    gs_out.CameraPosition = gs_in[1].CameraPosition;
    EmitVertex();

    // gl_Position = explode(gl_in[2].gl_Position, GetNormal());
    gl_Position = gl_in[2].gl_Position;
    gs_out.Position = gs_in[2].Position;
    gs_out.Normal = gs_in[2].Normal;
    gs_out.CameraPosition = gs_in[2].CameraPosition;
    EmitVertex();

    EndPrimitive();
}

// vec4 explode(vec4 position, vec3 normal) { return position + vec4(normal * u_Magnitude, 0.0); }