#include "ZeroVulkan.h"

int main() 
{
    ZeroVulkan::init();
    
    size_t obj_id = ZeroVulkan::ZRenderer::createObject();
 
    float dt = 0.f;
    bool quit = false;
    while (!quit) 
    {    
        quit = ZeroVulkan::handleInput();

        ZeroVulkan::update(dt);
        
        ZeroVulkan::draw();
    }


    ZeroVulkan::clear();

    printf("successfully cleared!\n");
    
    return 0;
}
