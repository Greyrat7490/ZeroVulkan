#include "scene.h"

using namespace ZeroVulkan;

void TestScene::start() { 
    float vertices[] = {
         0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,

         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,

        5, 1, 0,
        0, 4, 5,

        6, 3, 2,
        6, 7, 3,

        5, 2, 1,
        5, 6, 2,

        0, 3, 4,
        7, 4, 3,

        4, 7, 6,
        6, 5, 4
    };

    ZMesh mesh(vertices, sizeof(vertices)/sizeof(float), indices, sizeof(indices)/sizeof(uint32_t));

    ZShaderSet shaders("Test/shader/phong.vert", "Test/shader/phong.frag");
    
    ZObject& obj = createObject(shaders, mesh);

    mat4* proj     = obj.getUniformComponent<mat4>(0);
    mat4* view     = obj.getUniformComponent<mat4>(1);
    mat4* model    = obj.getUniformComponent<mat4>(2);
    vec3* lightDir = obj.getUniformComponent<vec3>(3);

    *model = mat4(1.f);
    *lightDir = vec3(-1.f, -1.f, 3.f);

    obj.set3DMats(proj, view, model);
    

    // if you just want to use the standard shader (phong shader)
    // you can simply use this:
    // createObject(mesh);
 

    createRect(vec2(0.5f, 0.5f), 0.25f, 0.25f, vec4(1.f, 0.f, 1.f, 1.f));
}
    
void TestScene::update(float dt) { 
    (void)dt;
}

void TestScene::end() {
    puts("test scene end");
}
