#include "ZeroVulkan.h"
#include "scene.h"

int main()
{
    ZeroVulkan::ZProject test;
    ZeroVulkan::ZScene::create<TestScene>();

    ZeroVulkan::ZShaderSet testShaderSet("Test/shader/test.vert", "Test/shader/test.frag");
    
    test.run();

    return 0;
}
