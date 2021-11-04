#include "ZeroVulkan.h"

#include "computeShader/scene.h"
#include "object/scene.h"
#include "primitives/scene.h"
#include "stencil/scene.h"

int main()
{
    ZeroVulkan::ZProject test;
    // ZeroVulkan::ZScene::create<ZStencilScene>();
    // ZeroVulkan::ZScene::create<ZObjectScene>();
    ZeroVulkan::ZScene::create<ComputeShaderScene>();
    // ZeroVulkan::ZScene::create<PrimitivesScene>();

    test.run();

    return 0;
}
