#include "ZeroVulkan.h"

#include <chrono>

#include "ZObject/ZObject.h"
#include "ZRenderer/ZRenderer.h"


namespace ZeroVulkan {
    uint8_t ZProject::m_counter;

    ZProject::ZProject() {
        if (m_counter) {
            puts("ERROR: you can only have one instance of ZProject");
            exit(1);
        }

#ifdef Z_DEBUG
        ZRenderer::printVulkanInfos();
#endif
        ZWindow::createWindow();
        ZRenderer::initRenderer();

        m_counter++;
    }

    ZProject::~ZProject() {
        ZWindow::clear();
        ZRenderer::clear();

        puts("successfully cleared!");
    }

    void ZProject::run() {
        float drawTimer = 0.f;
        float fixedDeltaTime = 0.002f; // 2ms
        float accumulator = 0.f;
        uint32_t fps = 0;
        bool quit = false;

        if (!ZScene::getSceneCount()) {
            puts("ERROR: no ZScene created so you cannot draw anything\n"
                 "  use 'ZScene::create<YourZSceneClass>();' to create one");
            return;
        }

        ZRenderer::start();


        auto last = std::chrono::high_resolution_clock::now();
        while (!quit)
        {
            auto current = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration_cast<std::chrono::microseconds>(current - last).count() / 1'000'000.f;
            last = current;
            accumulator += dt;
            drawTimer += dt;


            quit = ZWindow::handleEvents();

            ZRenderer::update(dt);
            while (accumulator >= fixedDeltaTime) {
                ZRenderer::fixedUpdate(fixedDeltaTime);
                accumulator -= fixedDeltaTime;
            }

            // TODO: maybe non blocking draw (not sure if this is a good idea / do research!)
            ZRenderer::drawFrame();


            if (drawTimer >= 0.25f) {
                ZeroVulkan::ZWindow::setTitle("fps: " + std::to_string(fps * 4) + " deltaTime: " + std::to_string(0.25f / fps) + "s");
                fps = 0;
                drawTimer = 0.f;
            }

            fps++;
        }


        ZRenderer::end();
    }
}
