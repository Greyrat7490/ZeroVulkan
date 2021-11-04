#version 450

layout( location = 0 ) in vec3 inPosition;

layout( location = 0 ) out vec4 outColor;

layout ( binding = 0 ) uniform UBO
{
	mat4 projection;
    mat4 view;
    mat4 model;
	vec4 color;
} ubo;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4( inPosition, 1.0 );
    outColor = ubo.color;
}
