#include "scene.h"

using namespace ZeroVulkan;

TestScene::TestScene() { 
    puts("created test scene");
}
    
void TestScene::start() { 
    puts("test scene start");
  
    /* ----------- init mesh ----------- */
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
    /* ----------------------------- */


    /* ----------- init ShaderSet ----------- */
    //TODO: shader parser to automaticly add Locations, Bindings and UniformComponents
    ZShaderSet shaders("Test/shader/phong.vert", "Test/shader/phong.frag");
 
    // setup Uniform ------------------------------
    // TODO: really cumbersome and not type save (to templated methode maybe)
    mat4* proj = std::any_cast<mat4>( shaders.uniformLayout.addComponent(ZType::MAT4) );
    mat4* view = std::any_cast<mat4>( shaders.uniformLayout.addComponent(ZType::MAT4) );
    mat4* model = std::any_cast<mat4>( shaders.uniformLayout.addComponent(ZType::MAT4) );
    vec3* lightDir = std::any_cast<vec3>( shaders.uniformLayout.addComponent(ZType::VEC3) );

    
    // update Uniform values ----------------------
    *model = mat4(1.f);
    *lightDir = vec3(1.f, 1.f, 3.f);
    // shaders.update(); // uniform is not yet created so this would cause an error(trigger assertion)
    /* ------------------------------------ */
    
    
    /* ----------- init ZObject ----------- */
    ZObject& obj = createObject(shaders, mesh);
    obj.set3DMats(proj, view);
    /* ------------------------------------ */
}

void TestScene::update(float dt) { 
    (void)dt;
}

void TestScene::end() {
    puts("test scene end");
}
