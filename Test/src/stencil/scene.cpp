#include "scene.h"
#include "types.h"


ZFirstPersonCam* cam = nullptr;


void ZStencilScene::start() {
    puts("ZStencilBuffer test scene start");

    cam = ZScene::createFirstPersonCam();
    cam->setPos(vec3(-2.f, 1.f, 0.7f));
    cam->setRot(vec2(2.f, 0.3f));

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
    ZStencil* stencil = new ZStencil();
    shaders.setStencil(stencil);

    ZObject& obj = createObject(shaders, mesh);

    objUbo = obj.getUniformStruct<Obj2UBO>(0, {
        ZType::MAT4,
        ZType::MAT4,
        ZType::MAT4,
        ZType::VEC3,
    });

    objUbo->model = mat4(1.f);
    objUbo->lightDir = vec3(-1.f, -1.f, 3.f);

    obj.set3DMats(&objUbo->proj, &objUbo->view, &objUbo->model);

    outlineUbo = obj.getStencilUniformStruct<OutlineUBO>(0, { ZType::MAT4, ZType::MAT4, ZType::MAT4 });
}

void ZStencilScene::update(float dt) {
    outlineUbo->model = objUbo->model;
    outlineUbo->view = objUbo->view;
    outlineUbo->proj = objUbo->proj;

    cam->rotate(vec2(0.f, 2.f) * dt);
}

void ZStencilScene::end() {
    puts("ZStencilBuffer test scene end");
}
