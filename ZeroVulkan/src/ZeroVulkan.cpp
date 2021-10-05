#include "ZeroVulkan.h"
#include "Window/window.h"
#include "ZRenderer/ZRenderer.h"

namespace ZeroVulkan {
    void init() {
#ifdef Z_DEBUG
        ZRenderer::printVulkanInfos();
#endif
        ZWindow::createWindow();
        ZRenderer::initRenderer();
    }

    void clear() {
        ZWindow::clear();
        ZRenderer::clear();

        printf("successfully cleared!\n");
    }
 
    void run() {
        float drawTimer = 0.f;
        float fixedDeltaTime = 0.002f; // 2ms
        float accumulator = 0.f;
        uint32_t fps = 0;
        bool quit = false;


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
            
            while (accumulator >= fixedDeltaTime) {
                ZRenderer::update(fixedDeltaTime);
                accumulator -= fixedDeltaTime;
            }
            
            // TODO: maybe non blocking draw (not sure if this is a good idea / do research!)
            ZRenderer::drawFrame();


            if (drawTimer >= 1.f) {
                ZeroVulkan::ZWindow::setTitle("fps: " + std::to_string(fps) + " deltaTime: " + std::to_string(1.f / fps) + "s");
                fps = 0;
                drawTimer = 0.f;
            }
        
            fps++;
        }


        ZRenderer::end();
    }
}
