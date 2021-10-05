#include "Window/window.h"
#include "ZShaders/ZShaders.h"
#include "ZeroVulkan.h"
#include <chrono>
#include <string>

#include "scene.h"

int main() 
{
    ZeroVulkan::init();

    ZeroVulkan::ZWindow::setTitle("Test");
    
    TestScene* scene = new TestScene;

    ZeroVulkan::run();

    ZeroVulkan::clear();

    return 0;
}
