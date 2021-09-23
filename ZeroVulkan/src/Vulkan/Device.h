#ifndef H_DEVICE
#define H_DEVICE

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include <vector>
#include <stdio.h>
#include "CommandBuffer.h"

namespace ZeroVulkan
{
	class ZDevice
	{
	public:
		ZDevice(const ZDevice&) = delete;

		static ZDevice& get()
		{
			static ZDevice d;
			return d;
		}

		static struct QueueFamilyIndices
		{
			int graphics = -1;
			int present = -1;
			int compute = -1;
		} queueFamilyIndices;

		// -------------------
		// Device
		// -------------------
		inline static VkDevice getDevice() { return get().m_dev; }
		inline static VkInstance getInstance() { return get().m_instance; }
		inline static VkPhysicalDevice* getPhysicalDev() { return get().m_physicalDevices; }
		inline static VkQueue& getQueue() { return get().m_queue; }
		// -------------------

		inline static void createCommandPool() { get().createCommandPoolImpl(); }

		inline static VkSampler& getSampler() { return get().m_sampler; }
		inline static ZCommandPool* getCommandPool() { return get().m_commandPool; }

		// -------------------
		// SyncObjects
		// -------------------
		inline static VkSemaphore& getSemaphoreImgAvailable() { return get().m_semaphoreImgAvailable; }
		inline static VkSemaphore& getSemaphoreRenderingDone() { return get().m_semaphoreRenderingDone; }
		inline static std::vector<VkFence>& getFences() { return get().m_inFlightFences; }
		// -------------------

		// -----------------------------
		// DepthBuffering
		// -----------------------------
		inline static VkFormat getDepthFormat() { return get().m_depthFormat; }
		inline static VkImage& getDepthImage() { return get().m_depthImage; }
		inline static VkImageView& getDepthImageView() { return get().m_depthImageView; }
		inline static VkDeviceMemory& getDepthImageMemory() { return get().m_depthImageMemory; }
		// -----------------------------

		// -------------------
		// Renderer
		// -------------------
		inline static VkFormat getFormat() { return get().m_format; }
		inline static VkFormat getImageViewFormat() { return get().m_imageViewFormat; }
		inline static VkRenderPass& getRenderPass() { return get().m_renderPass; }
		// -------------------

		// -------------------
		// Surface
		// -------------------
		inline static VkSurfaceKHR& getSurface() { return get().m_surface; }
		inline static VkSurfaceCapabilitiesKHR& getSurfaceCapabilities() { return get().m_surfaceCapabilities; }
		inline static VkSurfaceFormatKHR& getSurfaceFormat() { return get().m_surfaceFormat; }
		// -------------------

		// -------------------
		// Swapchain
		// -------------------
		inline static VkSwapchainKHR& getSwapchain() { return get().m_swapchain; }
		inline static std::vector<VkImage>& getSwapchainImages() { return get().m_swapchainImages; }
		inline static std::vector<VkImageView>& getSwapchainImageViews() { return get().m_swapchainImageViews; }

		inline static VkPresentModeKHR& getSwapchainPresentMode() { return get().m_swapchainPresentMode; }
		inline static VkFormat& getSwapchainImageFormat() { return get().m_swapchainImageFormat; }
		inline static VkExtent2D& getSwapchainExtent() { return get().m_swapchainExtent; }
		inline static std::vector<VkFramebuffer>& getSwapchainFramebuffers() { return get().m_swapchainFramebuffers; }

		inline static uint32_t& getCurFrame() { return get().curFrame; }
		inline static const int MAX_FRAMES_IN_FLIGHT = 2;
		// -------------------

	private:
		ZDevice();

		// -------------------
		// Device
		// -------------------
		VkDevice m_dev = nullptr;
		VkInstance m_instance = nullptr;
		VkPhysicalDevice* m_physicalDevices = nullptr;
		VkQueue m_queue = nullptr;
		// -------------------

		ZCommandPool* m_commandPool = nullptr; //default commandPool ( used for graphics queue )
		VkSampler m_sampler = nullptr;

		// -------------------
		// SyncObjects
		// -------------------
		VkSemaphore m_semaphoreImgAvailable = nullptr;
		VkSemaphore m_semaphoreRenderingDone = nullptr;
		std::vector<VkFence> m_inFlightFences = { nullptr };
		// -------------------

		// -----------------------------
		// DepthBuffering
		// -----------------------------
		const VkFormat m_depthFormat = VK_FORMAT_D32_SFLOAT;
		VkImage m_depthImage = nullptr;
		VkImageView m_depthImageView = nullptr;
		VkDeviceMemory m_depthImageMemory = nullptr;
		// -----------------------------

		// -------------------
		// Renderer
		// -------------------
		const VkFormat m_format = VK_FORMAT_B8G8R8A8_UNORM;
		const VkFormat m_imageViewFormat = VK_FORMAT_R8G8B8A8_UNORM;
		VkRenderPass m_renderPass = nullptr;
		// -------------------

		// -------------------
		// Surface
		// -------------------
		VkSurfaceKHR m_surface = nullptr;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		// -------------------

		// -------------------
		// Swapchain
		// -------------------
		VkSwapchainKHR m_swapchain = nullptr;
		std::vector<VkImage> m_swapchainImages = { nullptr };
		std::vector<VkImageView> m_swapchainImageViews = { nullptr };

		VkPresentModeKHR m_swapchainPresentMode;
		VkFormat m_swapchainImageFormat;
		VkExtent2D m_swapchainExtent;
		std::vector<VkFramebuffer> m_swapchainFramebuffers = { nullptr };

		uint32_t curFrame = 0;
		// -------------------


		void createInstanceImpl();
		void getPhysicalDevicesImpl();
		void createCommandPoolImpl();
	};
}

#endif // H_DEVICE
