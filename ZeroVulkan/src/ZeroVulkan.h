#ifndef ZERO_VULKAN_H_
#define ZERO_VULKAN_H_

#include "Window/window.h"

#include "utils.h"
#include "ZScene/ZScene.h"

#include "ZMesh/ZMesh.h"
#include "ZStencil/ZStencil.h"
#include "ZShaderSet/ZShaderSet.h"
#include "ZComputeShader/ZComputeShader.h"

#include "ZObject/ZObject.h"
#include "ZPrimitive/ZPrimitvie.h"


namespace ZeroVulkan {
    class ZProject {
    public:
        ZProject();
        ~ZProject();

        void run();
    private:
        // to check if only 1 instance of ZProject exists
        static uint8_t m_counter;
    };
}

#endif // ZERO_VULKAN_H_
