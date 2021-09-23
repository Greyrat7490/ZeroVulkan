#include "ZeroVulkan.h"
#include "Window/window.h"
#include "renderer.h"

namespace ZeroVulkan {
    void init() {
        ZRenderer::printVulkanInfos();
        ZWindow::createWindow();
        ZRenderer::initRenderer();
    }
    void clear() {
        ZWindow::clear();
    }
    
    void draw() {
        ZRenderer::drawFrame();
    }
    
    bool handleInput() {
        return ZWindow::handleEvents();
    }
    
    void update(float dt)   {
        ZRenderer::update(dt);
    }
}
