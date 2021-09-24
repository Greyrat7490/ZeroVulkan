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
