#version 450

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
    gl_Position = vec4(pos[gl_VertexIndex], 0.f, 1.f);
}

