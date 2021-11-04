#ifndef CORE_H_
#define CORE_H_

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>

#include <chrono>
#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>

#include "Image.h"
#include "Device.h"
#include "Buffer.h"
#include "Shader.h"
#include "Vertex.h"
#include "Memory.h"
#include "Uniform.h"
#include "Surface.h"
#include "Pipeline.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "SyncObjects.h"
#include "StorageBuffer.h"
#include "DescriptorSet.h"
#include "CommandBuffer.h"
#include "DepthBuffering.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"

#endif // CORE_H_
