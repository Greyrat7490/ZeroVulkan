#version 450

layout( binding = 0 ) uniform UBO
{
    mat4 projection;
    mat4 view;
    mat4 model;
	float time;
} ubo;

layout( location = 0 ) in vec3 v_position;
layout( location = 1 ) in vec3 v_color;
layout( location = 2 ) in vec2 v_texcoord;

layout( location = 0 ) out vec3 f_color;
layout( location = 1 ) out vec2 f_texcoord;
layout( location = 2 ) out float time;

void main(void)
{
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4( v_position, 1.0 );
	f_color = v_color;
	f_texcoord = v_texcoord;
	time = ubo.time;
}