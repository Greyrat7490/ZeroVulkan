#include "scene.h"

void PrimitivesScene::start() {
    puts("primitives test scene start");

    ZScene::createLookAtCam();

    createRect(vec2(0.5f, 0.5f), 0.25f, 0.25f, vec4(1.f, 0.f, 1.f, 1.f));
}

void PrimitivesScene::update(float dt) {
    (void) dt;
}

void PrimitivesScene::end() {
    puts("primitives test scene end");
}
