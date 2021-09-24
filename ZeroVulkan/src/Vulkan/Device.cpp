#include "Device.h"

namespace ZeroVulkan
{
	struct ZDevice::QueueFamilyIndices ZDevice::queueFamilyIndices;

    void ZDevice::clearImpl() {
		vkDeviceWaitIdle(m_dev);
        
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			vkDestroyFence(m_dev, m_inFlightFences[i], nullptr);

		vkDestroySemaphore(m_dev, m_semaphoreImgAvailable, nullptr);
		vkDestroySemaphore(m_dev, m_semaphoreRenderingDone, nullptr);

		delete getCommandPool();

		for (uint32_t i = 0; i < m_swapchainImageViews.size(); i++ )
			vkDestroyFramebuffer(m_dev, m_swapchainFramebuffers[i], nullptr);

		vkDestroyImageView(m_dev, m_depthImageView, nullptr);
		vkDestroyImage(m_dev, m_depthImage, nullptr);
		vkFreeMemory(m_dev, m_depthImageMemory, nullptr);

		vkDestroyRenderPass(m_dev, m_renderPass, nullptr);

		vkDestroySampler(m_dev, m_sampler, nullptr);

		for (uint32_t i = 0; i < m_swapchainImageViews.size(); i++)
			vkDestroyImageView(m_dev, m_swapchainImageViews[i], nullptr);

		vkDestroySwapchainKHR(m_dev, m_swapchain, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyDevice(m_dev, nullptr);

		vkDestroyInstance(m_instance, nullptr);

		delete[] m_physicalDevices;
    }
    
	ZDevice::ZDevice()
	{
		createInstanceImpl();
		getPhysicalDevicesImpl();

		float queuePriority[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		VkDeviceQueueCreateInfo devQueueCreateInfo;
		devQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		devQueueCreateInfo.pNext = nullptr;
		devQueueCreateInfo.flags = 0;
		devQueueCreateInfo.queueFamilyIndex = 0;    // choose proper index
		devQueueCreateInfo.queueCount = 1;// check how many queues are avaible
		devQueueCreateInfo.pQueuePriorities = queuePriority;

		VkPhysicalDeviceFeatures usedFeatures = {};
		usedFeatures.samplerAnisotropy = true;
		usedFeatures.fillModeNonSolid = true;
		usedFeatures.wideLines = true;

		const std::vector<const char*> devExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDeviceCreateInfo devCreateInfo;
		devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		devCreateInfo.pNext = nullptr;
		devCreateInfo.flags = 0;
		devCreateInfo.queueCreateInfoCount = 1;
		devCreateInfo.pQueueCreateInfos = &devQueueCreateInfo;
		devCreateInfo.enabledLayerCount = 0;
		devCreateInfo.ppEnabledLayerNames = nullptr;
		devCreateInfo.enabledExtensionCount = static_cast<uint32_t>(devExtensions.size());
		devCreateInfo.ppEnabledExtensionNames = devExtensions.data();
		devCreateInfo.pEnabledFeatures = &usedFeatures;

		VkResult res = vkCreateDevice(m_physicalDevices[0], &devCreateInfo, nullptr, &m_dev);
        if (res != VK_SUCCESS)
            printf("create device ERROR: %d\n", res);

		//change later to a proper index
		vkGetDeviceQueue(m_dev, 0, 0, &m_queue);

		printf("created ZkDevice\n");
	}

	void ZDevice::getPhysicalDevicesImpl()
	{
		uint32_t devCount;
		vkEnumeratePhysicalDevices(m_instance, &devCount, nullptr);

		m_physicalDevices = new VkPhysicalDevice[devCount];
		vkEnumeratePhysicalDevices(m_instance, &devCount, m_physicalDevices);
	}

	void ZDevice::createInstanceImpl()
	{
		const char* validationLayers[1] = {
            "VK_LAYER_KHRONOS_validation"
		};

		const char* KHRSurfaceExtension[2] = {
			"VK_KHR_surface", "VK_KHR_xcb_surface"
		};

		VkApplicationInfo appInfo = {};

		//discripes the type of the Struct ( because of a void* -> needs discription )
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "first App";
		appInfo.applicationVersion = VK_MAKE_VERSION( 0, 0, 1 );
		appInfo.pEngineName = "ZeroVulkan";
		appInfo.engineVersion = VK_MAKE_VERSION( 0, 0, 1 );
		//min. requirement of API-Version
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		//Layers are some steps you can put between proccess` (i.e. err-handling, profiling, ...)
		instanceInfo.enabledLayerCount = 1;
		instanceInfo.ppEnabledLayerNames = validationLayers;
		instanceInfo.enabledExtensionCount = 2;
		instanceInfo.ppEnabledExtensionNames = KHRSurfaceExtension;

		VkResult res = vkCreateInstance(&instanceInfo, nullptr, &m_instance);
        if (res != VK_SUCCESS )
            printf("create instance ERROR: %d\n", res);
	}

	void ZDevice::createCommandPoolImpl()
	{
		m_commandPool = new ZCommandPool();
	}
}
