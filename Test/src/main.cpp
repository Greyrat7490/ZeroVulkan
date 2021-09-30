#include "Window/window.h"
#include "ZeroVulkan.h"
#include <chrono>
#include <string>

int main() 
{
    ZeroVulkan::init();

    ZeroVulkan::ZWindow::setTitle("Test");
    
    ZeroVulkan::ZScene::current().createObject();
 
    float drawTimer = 0.f;
    float fixedDeltaTime = 0.002f; // 2ms
    float accumulator = 0.f;
    uint32_t fps = 0;

    auto last = std::chrono::high_resolution_clock::now();
    
    bool quit = false;
    while (!quit) 
    {
        auto current = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration_cast<std::chrono::microseconds>(current - last).count() / 1'000'000.f;
        last = current;
        accumulator += dt;
        drawTimer += dt;
        

        quit = ZeroVulkan::handleInput();
        
        while (accumulator >= fixedDeltaTime) {
            ZeroVulkan::update(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }
        
        // TODO: maybe non blocking draw (not sure if this is a good idea / do research!)
        ZeroVulkan::draw();


        if (drawTimer >= 1.f) {
            ZeroVulkan::ZWindow::setTitle("fps: " + std::to_string(fps) + " deltaTime: " + std::to_string(1.f / fps) + "s");
            fps = 0;
            drawTimer = 0.f;
        }
    
        fps++;
    }


    ZeroVulkan::clear();

    printf("successfully cleared!\n");
    
    return 0;
}
