#include "Device.h"

#include "Surface.h"
#include "Swapchain.h"
#include "SyncObjects.h"


namespace ZeroVulkan::ZDevice {
    // -------------------
    // Device
    // -------------------
    static VkDevice s_dev = nullptr;
    static VkInstance s_instance = nullptr;
    static VkPhysicalDevice* s_physicalDevices = nullptr;
    static VkQueue s_queue = nullptr;
    // -------------------

    static ZCommandPool* s_commandPool = nullptr; //default commandPool ( used for graphics queue )
    static VkSampler s_sampler = nullptr;

    // -------------------
    // Device
    // -------------------
    VkDevice getDevice() { return s_dev; }
    VkInstance getInstance() { return s_instance; }
    VkPhysicalDevice* getPhysicalDev() { return s_physicalDevices; }
    VkQueue& getQueue() { return s_queue; }
    // -------------------


    VkSampler& getSampler() { return s_sampler; }
    ZCommandPool* getCommandPool() { return s_commandPool; }

    void clear() {
        vkDeviceWaitIdle(s_dev);

        SyncObjects::clear();

        delete s_commandPool;

        vkDestroySampler(s_dev, s_sampler, nullptr);

        Swapchain::clear();
        Surface::clear();

        vkDestroyDevice(s_dev, nullptr);
        vkDestroyInstance(s_instance, nullptr);

        delete[] s_physicalDevices;
    }

    void createInstance();
    void getPhysicalDevices();

    void init()
    {
        createInstance();
        getPhysicalDevices();

        float queuePriority[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        VkDeviceQueueCreateInfo devQueueCreateInfo;
        devQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        devQueueCreateInfo.pNext = nullptr;
        devQueueCreateInfo.flags = 0;
        devQueueCreateInfo.queueFamilyIndex = 0;    //TODO: choose proper index
        devQueueCreateInfo.queueCount = 1;          //TODO: check how many queues are avaible
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

        VkResult res = vkCreateDevice(s_physicalDevices[0], &devCreateInfo, nullptr, &s_dev);
        if (res != VK_SUCCESS)
            printf("create device ERROR: %d\n", res);

        //change later to a proper index
        vkGetDeviceQueue(s_dev, 0, 0, &s_queue);

        printf("created ZDevice\n");

        s_commandPool = new ZCommandPool();
    }

    void getPhysicalDevices()
    {
        uint32_t devCount;
        vkEnumeratePhysicalDevices(s_instance, &devCount, nullptr);

        s_physicalDevices = new VkPhysicalDevice[devCount];
        vkEnumeratePhysicalDevices(s_instance, &devCount, s_physicalDevices);
    }

    void createInstance()
    {
#ifdef Z_DEBUG
        const char* layerNames[1] = {
            "VK_LAYER_KHRONOS_validation"
        };
        const uint32_t layerCount = 1;
#else
        const char** layerNames = nullptr;
        const uint32_t layerCount = 0;
#endif

        const char* KHRSurfaceExtension[2] = {
            "VK_KHR_surface", "VK_KHR_xlib_surface"
        };

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "App";
        appInfo.applicationVersion = VK_MAKE_VERSION( 0, 0, 1 );
        appInfo.pEngineName = "ZeroVulkan";
        appInfo.engineVersion = VK_MAKE_VERSION( 0, 0, 1 );
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledLayerCount = layerCount;
        instanceInfo.ppEnabledLayerNames = layerNames;
        instanceInfo.enabledExtensionCount = 2;
        instanceInfo.ppEnabledExtensionNames = KHRSurfaceExtension;

        VkResult res = vkCreateInstance(&instanceInfo, nullptr, &s_instance);
        if (res != VK_SUCCESS )
            printf("create instance ERROR: %d\n", res);
    }
}
