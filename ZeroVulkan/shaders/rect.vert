#version 450

layout(binding = 0) uniform UBO
{
    vec2 pos;
    vec2 size;
    vec4 color;
} ubo;

layout (location = 0) out vec4 outColor;

vec2 pos[6] = vec2[] (
     vec2(-1.f, -1.f),
     vec2( 1.f, -1.f),
     vec2( 1.f,  1.f),

     vec2(-1.f, -1.f),
     vec2( 1.f,  1.f),
     vec2(-1.f,  1.f)
);

void main()
{
    vec2 pos = pos[gl_VertexIndex] * ubo.size - vec2(1.f) + ubo.pos * 2.f;
    
    gl_Position = vec4(pos, 0.f, 1.f);
    outColor = ubo.color;
}
