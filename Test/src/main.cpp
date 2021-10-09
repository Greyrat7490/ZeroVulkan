#include "ZeroVulkan.h"
#include "scene.h"

int main()
{
    // no window is created so this will only print an ERROR
    // ZeroVulkan::ZWindow::setTitle("Test");

    ZeroVulkan::ZProject test;
    ZeroVulkan::ZScene::create<TestScene>();

    test.run();

    return 0;
}
