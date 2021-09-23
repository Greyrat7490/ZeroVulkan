#ifndef H_STENCILE_BUFFER
#define H_STENCILE_BUFFER

#include <string>
#include "UniformBuffer.h"
#include "types.h"

namespace ZeroVulkan
{
	class ZVertexLayout;
	class ZDescriptorSetLayout;
	class ZDescriptorPool;

	class ZStencilBuffer
	{
	public:
		ZStencilBuffer();
		~ZStencilBuffer();

		void createDescSet(const VkDescriptorPool& descriptorPool);
		void createPipelines(ZVertexLayout* vertexLayout, VkShaderModule& shaderModuleVert, VkShaderModule& shaderModuleFrag, VkDescriptorSetLayout* descLayouts, uint32_t layoutsCount);

		inline void updateUniform(mat4 projection, mat4 view, mat4 model) {m_uniform->update(0, projection, view, model); }

		inline const VkPipelineLayout& getPipelineLayout() const { return m_stencilLayout; }
		inline const VkPipeline& getPipeline() const { return m_stencil; }

		inline const VkPipelineLayout& getOutlinePipelineLayout() const { return m_outlineLayout; }
		inline const VkPipeline& getOutlinePipeline() const { return m_outline; }
		inline const VkDescriptorSet& getOutlineDescSet() const { return m_outlineDescSet; }
		inline const ZDescriptorSetLayout* getOutlineDescSetLayout() const { return m_outlineDescLayout; }
	private:
		void createUniform();

		const std::string m_outlineVertPath;
		const std::string m_outlineFragPath;

		VkShaderModule m_outlineVertModule;
		VkShaderModule m_outlineFragModule;

		ZUniform* m_uniform = nullptr;
		ZUniformLayout* m_uniformLayout = nullptr;

		ZDescriptorSetLayout* m_outlineDescLayout = nullptr;
		VkDescriptorSet m_outlineDescSet;

		VkPipelineLayout m_stencilLayout;
		VkPipelineLayout m_outlineLayout;

		VkPipeline m_stencil;
		VkPipeline m_outline;
	};
}

#endif // H_STENCILE_BUFFER
