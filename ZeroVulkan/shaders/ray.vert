#version 450

layout ( location = 0 ) in vec3 v_pos;

layout( binding = 0 ) uniform UBO
{
    mat4 projection;
    mat4 view;
} ubo;

void main()
{
    gl_Position = ubo.projection * ubo.view * vec4( v_pos, 1.0 );
}