#include "scene.h"
#include "types.h"

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
    //
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


    // createRect(vec2(0.5f, 0.5f), 0.25f, 0.25f, vec4(1.f, 0.f, 1.f, 1.f));



    struct alignas(16) Particle
    {
        vec4 pos;
        vec4 vel;
        vec4 color;
        float size;
        float lifeTime;
    };


    static uint32_t PARTICLE_COUNT = 100'000;

    std::vector<Particle> particles(PARTICLE_COUNT);

    ZComputeShader* compShader = new ZComputeShader("Test/shader/particle.comp", PARTICLE_COUNT);

    // not 100% safe but way better than no type checking
    ZType layout[] = {
        ZType::VEC4,
        ZType::VEC4,
        ZType::VEC4,
        ZType::FLOAT,
        ZType::FLOAT,
        ZType::FLOAT,
        ZType::FLOAT,
        ZType::FLOAT,
    };
    
    ubo = compShader->uniformBuffer.getStruct<ParticleSystemUBO>(layout, sizeof(layout)/sizeof(ZType));
    ubo->startColor = { 2.f, 1.f, 1.f, 1.f };
    ubo->endColor = { 1.f, 0.f, 1.f, 1.f };
    ubo->startPos = { 0.f, 0.f, 0.f, 1.f };
    ubo->startSize = 16.f;
    ubo->endSize = 2.f;
    ubo->lifeTime = 5.f;
    ubo->spawnDelay = 0.25f * 3.f / PARTICLE_COUNT; // good value around: lifetime / PARTICLE_COUNT

    for (Particle& p : particles) {
        p.color = { 1.f, 0.f, 1.f, 1.f };
        p.vel = vec4(rndFloat(-1.f, 1.f), rndFloat(-1.f, 1.f), 0.f, 0.f) * 0.25f;
        p.pos = vec4(0.f, 0.f, 0.f, 1.f);
        p.lifeTime = 0.f;
        p.size = 8.f;
    }
    // TODO: type safe
    compShader->storageBuffer.set(particles.data());


    ZShaderSet& particleSystem = createShaderSet("Test/shader/gpuParticle.vert", "Test/shader/gpuParticle.frag");
    particleSystem.setComputeShader(compShader);
    particleSystem.setTopology(ZTopology::POINT);
    // particleSystem.setTopology(ZTopology::LINE); // this looks really cool
}
    
void TestScene::update(float dt) { 
    ubo->deltaTime = dt;
}

void TestScene::end() {
    puts("test scene end");
}
