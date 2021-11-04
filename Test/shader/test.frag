#version 450

layout( location = 0 ) in vec3 v_color;
layout( location = 1 ) in vec2 v_texcoord;
layout( location = 2 ) in float time;

layout( location = 0 ) out vec4 fragColor;

void main(void)
{
    vec2 uv = 4 * v_texcoord;

    fragColor = vec4(
        10 * sin( cos( time+0.5 * uv.y ) * 40 * uv.x+time ),
        5 * cos( sin( time+0.1 * uv.x ) * 2 * uv.y+time ),
        1.0,
        1.0
    );
}
