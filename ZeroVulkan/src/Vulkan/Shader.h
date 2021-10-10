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

    // TODO: only compile if file changed
    inline std::string compileShader(const std::string& path) {
        #ifdef __linux
            std::string base = path.substr(0, path.find_last_of('/')) + "/compiled";
            std::string dest = base + path.substr(path.find_last_of('/')) + ".spv";

            createPath(base);
            
            std::string cmd = "glslangValidator -V " + path + " -o " + dest;
            std::system(cmd.c_str());

            return dest;
        #elif __APPLE__
            puts("TODO: test if glslangValidator is available in OSX");
            
            std::string base = path.substr(0, path.find_last_of('/')) + "/compiled";
            std::string dest = base + path.substr(path.find_last_of('/')) + ".spv";
            
            createPath(base);
            
            std::string cmd = "glslangValidator -V " + path + " -o " + dest;
            std::system(cmd.c_str());

            return dest;
        #elif __WINDOWS_PLATFORM
            puts("TODO: test filesystem path && change to correct cmd");
            
            std::u16string base = path.substr(0, path.find_last_of('/')) + "/compiled";
            std::u16string dest = base + path.substr(path.find_last_of('/')) + ".spv";
            
            createPath(base);
            
            // std::string cmd = "glslangValidator -V " + path + " -o " + dest;
            // std::system(cmd.c_str());

            return dest;
        #else
            puts("this API will only support Linux, OSX and Windows");
            exit(1);
        #endif
    }
    
	inline void createShaderModule(const std::string& path, VkShaderModule* shaderModule)
	{
        std::string source = readFile(compileShader(path));
        
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
