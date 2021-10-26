#include "scene.h"

using namespace ZeroVulkan;

float* uboDt = nullptr;

void TestScene::start() { 
    // float vertices[] = {
    //      0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
    //      0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
    //     -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
    //
    //      0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
    // };
    //
    // uint32_t indices[] = {
    //     0, 1, 2,
    //     2, 3, 0,
    //
    //     5, 1, 0,
    //     0, 4, 5,
    //
    //     6, 3, 2,
    //     6, 7, 3,
    //
    //     5, 2, 1,
    //     5, 6, 2,
    //
    //     0, 3, 4,
    //     7, 4, 3,
    //
    //     4, 7, 6,
    //     6, 5, 4
    // };
    //
    // ZMesh mesh(vertices, sizeof(vertices)/sizeof(float), indices, sizeof(indices)/sizeof(uint32_t));
    //
    // ZShaderSet shaders("Test/shader/phong.vert", "Test/shader/phong.frag");
    //
    // ZObject& obj = createObject(shaders, mesh);
    //
    // mat4* proj     = obj.getUniformComponent<mat4>(0);
    // mat4* view     = obj.getUniformComponent<mat4>(1);
    // mat4* model    = obj.getUniformComponent<mat4>(2);
    // vec3* lightDir = obj.getUniformComponent<vec3>(3);
    //
    // *model = mat4(1.f);
    // *lightDir = vec3(-1.f, -1.f, 3.f);
    //
    // obj.set3DMats(proj, view, model);
    

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
    
    ZComputeShader* compShader = new ZComputeShader(PARTICLE_COUNT);

    compShader->createStorageBuffer(sizeof(Particle) * PARTICLE_COUNT);

    // TODO: addComponents() -> takes an array
    compShader->uniformBuffer->addComponent(ZType::VEC4);
    compShader->uniformBuffer->addComponent(ZType::VEC4);
    compShader->uniformBuffer->addComponent(ZType::VEC4);
    compShader->uniformBuffer->addComponent(ZType::FLOAT);
    compShader->uniformBuffer->addComponent(ZType::FLOAT);
    compShader->uniformBuffer->addComponent(ZType::FLOAT);
    compShader->uniformBuffer->addComponent(ZType::FLOAT);
    compShader->uniformBuffer->addComponent(ZType::FLOAT);
    compShader->uniformBuffer->create();

    // TODO: get a struct
    *compShader->uniformBuffer->getComponent<vec4>(0) = { 1.f, 1.f, 1.f, 1.f };
    *compShader->uniformBuffer->getComponent<vec4>(1) = { 1.f, 0.f, 1.f, 1.f };
    *compShader->uniformBuffer->getComponent<vec4>(2) = { 0.f, 0.f, 0.f, 1.f };
    *compShader->uniformBuffer->getComponent<float>(3) = 16.f;
    *compShader->uniformBuffer->getComponent<float>(4) = 2.f;
    *compShader->uniformBuffer->getComponent<float>(5) = 3.f;
    *compShader->uniformBuffer->getComponent<float>(6) = 0.0001f;
    uboDt = compShader->uniformBuffer->getComponent<float>(7);

    // TODO: parse ComputeShader
    compShader->descriptorSetLayout->addBinding(0, compShader->storageBuffer->bufferInfo, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
    compShader->descriptorSetLayout->addBinding(1, compShader->uniformBuffer->getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
    compShader->descriptorSetLayout->create();

    compShader->descriptorPool->addDescriptorLayout(compShader->descriptorSetLayout);
    compShader->descriptorPool->create();

    compShader->descriptorSet = new ZDescriptorSet;
    compShader->descriptorSet->create(compShader->descriptorSetLayout, compShader->descriptorPool->descriptorPool);

    for (Particle& p : particles) {
        p.color = { 1.f, 0.f, 1.f, 1.f };
        p.vel = vec4(rndFloat(-1.f, 1.f) * 0.25f, rndFloat(-1.f, 1.f) * 0.25f, 0.f, 0.f);
        p.pos = vec4(0.f, 0.f, 0.f, 1.f);
        p.lifeTime = 0.f;
        p.size = 8.f;
    }

    compShader->create();
    compShader->setStorageBuffer(particles.data());

    ZShaderSet& particleShaders = createShaderSet("Test/shader/gpuParticle.vert", "Test/shader/gpuParticle.frag");
    particleShaders.setComputeShader(compShader);
    particleShaders.setTopology(ZTopology::POINT);
    // particleShaders.setTopology(ZTopology::LINE); // this looks really cool
}
    
void TestScene::update(float dt) { 
    *uboDt = dt;
}

void TestScene::end() {
    puts("test scene end");
}
