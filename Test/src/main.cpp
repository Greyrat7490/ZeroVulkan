#include "ZeroVulkan.h"
#include "scene.h"

int main()
{
    ZeroVulkan::ZProject test;

    ZeroVulkan::ZWindow::setTitle("Test");
    ZeroVulkan::ZScene::create<TestScene>();

    test.run();

    return 0;
}
