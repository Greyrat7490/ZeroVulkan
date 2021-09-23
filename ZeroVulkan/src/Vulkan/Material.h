#ifndef H_MATERIAL
#define H_MATERIAL

#include <string>
#include <vector>
#include "UniformBuffer.h"
#include "Vertex.h"
#include "Pipeline.h"
#include "StencilBuffer.h"
#include "DescriptorSet.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"

namespace ZeroVulkan
{
	class ZMaterial
	{
	public:
		ZMaterial(const std::string& vertexShaderName, const std::string& fragmentShaderName);
		ZMaterial(const std::string& vertexShaderName, const std::string& fragmentShaderName, const std::string& texturePath);
		virtual ~ZMaterial();

		void update(float deltaTime);

		void create(ZUniform* uniform, bool debug, bool triangleTopology = true);

		void setShader(const std::string& shaderName, shaderType type);

		void bind(VkCommandBuffer& cmdBuffer) const;

		ZStencilBuffer* stencilBuffer = nullptr;

		VkPipeline pipeline = nullptr;
		VkPipelineLayout pipelineLayout = nullptr;

		ZDescriptorPool* descPool = nullptr;
		ZDescriptorSetLayout* descSetLayout = nullptr;
		ZDescriptorSet* descriptorSet = nullptr;

		ZVertexLayout* vertexLayout = nullptr;

		float counter = 0; // for shader animations
	private:
		VkShaderModule shaderModuleVert = nullptr;
		VkShaderModule shaderModuleFrag = nullptr;
	};
}

#endif // H_MATERIAL
