#include "Window/window.h"
#include "ZeroVulkan.h"
#include <chrono>
#include <cstdint>
#include <string>

int main() 
{
    ZeroVulkan::init();

    ZeroVulkan::ZWindow::setTitle("Test");
    
    ZeroVulkan::ZScene::current().createObject();
 
    float dt = 0.f;
    float timer = 0.f;
    
    bool quit = false;
    while (!quit) 
    {    
        auto start = std::chrono::high_resolution_clock::now();
        
        quit = ZeroVulkan::handleInput();
        ZeroVulkan::update(dt);
        ZeroVulkan::draw();

        
        timer += dt;
        
        if (timer >= 1) {
            ZeroVulkan::ZWindow::setTitle("fps: " + std::to_string(1.f / dt) + " deltaTime: " + std::to_string(dt) + "s");
            timer = 0;
        }    
    
        auto end = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.f;   
    }


    ZeroVulkan::clear();

    printf("successfully cleared!\n");
    
    return 0;
}
