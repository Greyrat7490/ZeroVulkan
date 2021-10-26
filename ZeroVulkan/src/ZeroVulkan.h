#ifndef ZERO_VULKAN_H_
#define ZERO_VULKAN_H_

#include "Window/window.h"
#include "ZObject/ZObject.h"
#include "ZMesh/ZMesh.h"
#include "ZShaderSet/ZShaderSet.h"
#include "Vulkan/ComputeShader.h"
#include "ZScene/ZScene.h"
#include "utils.h"

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
