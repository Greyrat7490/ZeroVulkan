#ifndef H_SHADER
#define H_SHADER

#include <vector>
#include <stdio.h>
#include <string>
#include <utils.h>
#include "Device.h"

namespace ZeroVulkan
{
    #define SHADER_TYPE_COUNT 3
	enum class ZShaderType
	{
		VERTEX,
		FRAGMENT,
        COMPUTE
	};

    std::string compileShader(const std::string& path);
	void createShaderModule(const std::string& path, VkShaderModule* shaderModule);
}

#endif // H_SHADER
