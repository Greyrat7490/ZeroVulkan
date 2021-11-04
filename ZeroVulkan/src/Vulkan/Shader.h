#ifndef SHADER_H_
#define SHADER_H_

#include <vulkan/vulkan_core.h>
#include <string>

namespace ZeroVulkan
{
    std::string compileShader(const std::string& path);
    void createShaderModule(const std::string& path, VkShaderModule* shaderModule);
}

#endif // SHADER_H_
