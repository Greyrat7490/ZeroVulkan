#ifndef H_SHADER
#define H_SHADER

#include <vector>
#include <stdio.h>
#include <string>
#include "Device.h"
#include "tools.h"

namespace ZeroVulkan
{
	enum class shaderType
	{
		VERTEX,
		FRAGMENT,
        COMPUTE
	};

	inline void createShaderModule(const std::string& path, VkShaderModule* shaderModule)
	{
		std::string source = readFile(path);

		VkShaderModuleCreateInfo shaderCreateInfo;
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.pNext = nullptr;
		shaderCreateInfo.flags = 0;
		shaderCreateInfo.codeSize = source.size();
		shaderCreateInfo.pCode = (uint32_t*)source.data();

		VkResult res = vkCreateShaderModule(ZDevice::getDevice(), &shaderCreateInfo, nullptr, shaderModule);
        if (res != VK_SUCCESS)
            printf("create shader module ERROR: %d\n", res);
	}
}

#endif // H_SHADER
