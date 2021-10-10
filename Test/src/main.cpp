#include "ZeroVulkan.h"
#include "scene.h"
#include "utils.h"
#include <string>

int main()
{
    // ASSERT_LINE(false, "test1")
    // ASSERT_FUNC(false, "test1")
    // ZeroVulkan::zassert(false, "test assert", __FILE__, __LINE__);
    // ZeroVulkan::zassert(false, "test assert2", __FILE__, __PRETTY_FUNCTION__);
    
    ZeroVulkan::ZProject test;
    ZeroVulkan::ZScene::create<TestScene>();

    test.run();

    return 0;
}
