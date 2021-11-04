#version 450

layout ( location = 0 ) in vec3 v_pos;
layout ( location = 1 ) in vec3 v_color;

layout( location = 0 ) out vec3 f_Color;
layout( location = 1 ) out vec3 f_Normal;
layout( location = 2 ) out vec3 f_ViewPos;
layout( location = 3 ) out vec3 f_LightPos;

layout( binding = 0 ) uniform UBO
{
    mat4 projection;
    mat4 view;
    mat4 model;
    vec3 lightPos;
} ubo;

void main()
{
    vec4 worldPos = ubo.model * vec4( v_pos, 1.0 );
    gl_Position = ubo.projection * ubo.view * worldPos;

    f_Color = v_color;
    f_Normal = normalize( mat3( ubo.model ) * v_pos );
    f_ViewPos = normalize( ubo.view * worldPos ).xyz;
    f_LightPos = normalize( ubo.lightPos - worldPos.xyz );
}
