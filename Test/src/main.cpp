#include "ZeroVulkan.h"
#include "scene.h"
#include "utils.h"

int main()
{
    printf("rng: %f\n", ZeroVulkan::rndFloat(0.f, 10.f));
    printf("rng: %d\n", ZeroVulkan::rndInt(0.f, 10.f));
    
    ZeroVulkan::ZProject test;
    ZeroVulkan::ZScene::create<TestScene>();

    test.run();

    return 0;
}
