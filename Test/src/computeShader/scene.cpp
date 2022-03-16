#include "scene.h"

void ComputeShaderScene::start() {
    puts("compute shader test scene start");

    static uint32_t PARTICLE_COUNT = 100'000;

    struct alignas(16) Particle
    {
        vec4 pos;
        vec4 vel;
        vec4 color;
        float size;
        float lifeTime;
    };
    std::vector<Particle> particles(PARTICLE_COUNT);


    ZComputeShader* compShader = new ZComputeShader("Test/shader/particle.comp", PARTICLE_COUNT);

    // not 100% safe but way better than no type checking
    psUbo = compShader->uniformBuffer.getStruct<ParticleSystemUBO>({
        ZType::VEC4,
        ZType::VEC4,
        ZType::VEC4,
        ZType::FLOAT,
        ZType::FLOAT,
        ZType::FLOAT,
        ZType::FLOAT,
        ZType::FLOAT,
    });
    psUbo->startColor = { 2.f, 1.f, 1.f, 1.f };
    psUbo->endColor = { 1.f, 0.f, 1.f, 1.f };
    psUbo->startPos = { 0.f, 0.f, 0.f, 1.f };
    psUbo->startSize = 16.f;
    psUbo->endSize = 2.f;
    psUbo->lifeTime = 5.f;
    psUbo->spawnDelay = 0.25f * 3.f / PARTICLE_COUNT; // good value around: lifetime / PARTICLE_COUNT

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

void ComputeShaderScene::update(float dt) {
    psUbo->deltaTime = dt;

    if (ZInput::isPressed(38)) // A
        puts("pressed 'A'");

    if (ZInput::isHold(38))
        puts("hold 'A'");

    if (ZInput::isReleased(38))
        puts("released 'A'");

    if (ZInput::isMouseWheelUp())
        puts("mouse wheel up");

    if (ZInput::isMouseWheelDown())
        puts("mouse wheel down");

    if (ZInput::isMouseMoving())
        printf("%d %d\n", (int)ZInput::getMouseMotion()[0], (int)ZInput::getMouseMotion()[1]);
        // printf("%u %u\n", (uint)ZInput::getMousePos()[0], (uint)ZInput::getMousePos()[1]);
}

void ComputeShaderScene::end() {
    puts("compute shader test scene end");
}
