#ifndef RENDERER_H_
#define RENDERER_H_

namespace ZeroVulkan::ZRenderer {
    void printVulkanInfos();
   
    void record();

    void initRenderer();
   
    void resizing();
    
    void drawFrame();

    void start();
    void end();
    void update(float dt);

    void clear();

    void updateWinSize();
}

#endif // RENDERER_H_
