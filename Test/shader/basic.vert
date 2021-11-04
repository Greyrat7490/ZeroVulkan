#version 450

out gl_PerVertex
{
    vec4 gl_Position;
};

layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec3 color;

layout( location = 0 ) out vec3 fragColor;

layout( binding = 0 ) uniform UBO// Uniform Buffer Object
{
    mat4 projection;
    mat4 view;
    mat4 model;
} ubo;

void main()
{                   // order is important in Matrix-multiplication
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4( pos, 1.0 );
    fragColor = color;
}
