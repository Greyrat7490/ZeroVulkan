#include "scene.h"

void ZObjectScene::start() {
    puts("ZObject test scene start");

    ZLookAtCam& cam = ZScene::createLookAtCam();
    cam.setPos(vec3(-2.f, 1.f, 0.7f));
    cam.lookAt(vec3(0.f));

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

    ZShaderSet shaders("ZeroVulkan/shaders/phong.vert", "ZeroVulkan/shaders/phong.frag");

    ZObject& obj = createObject(shaders, mesh);

    objUbo = obj.getUniformStruct<ObjUBO>(0, {
        ZType::MAT4,
        ZType::MAT4,
        ZType::MAT4,
        ZType::VEC3,
    });

    objUbo->model = mat4(1.f);
    objUbo->lightDir = vec3(-1.f, -1.f, 3.f);

    obj.set3DMats(&objUbo->proj, &objUbo->view, &objUbo->model);


    // if you just want to use the standard shader (phong shader)
    // you can simply use this:
    // createObject(mesh);
}

void ZObjectScene::update(float dt) {
    (void) dt;
}

void ZObjectScene::end() {
    puts("ZObject test scene end");
}
