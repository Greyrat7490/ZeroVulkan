#include "scene.h"

TestScene::TestScene() { 
    puts("created test scene");
}
    
void TestScene::start() { 
    puts("test scene start");
    
    ZeroVulkan::ZShaders shaders("Test/shader/compiled/phong.vert.spv", "Test/shader/compiled/phong.frag.spv");
    ZeroVulkan::ZMesh mesh;
    
    createObject(shaders, mesh);
}

void TestScene::update(float dt) {

}

void TestScene::end() {
    puts("test scene end");
}
