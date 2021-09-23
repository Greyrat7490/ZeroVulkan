#include <cstdint>
#include <vector>
#include "Window/window.h"
#include "renderer.h"
#include "Vulkan/ZvObject.h"
#include "Vulkan/Scene.h"

namespace ZeroVulkan::ZRenderer {
    uint32_t win_width, win_height;
    VkViewport viewport;
    VkRect2D scissor;
    mat4 proj;
    
    std::vector<ZObject> objects;
    
    size_t createObject() {
        objects.emplace_back();
        record();
        return objects.size() - 1;
    }
    
    void printVulkanInfos() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        VkLayerProperties* layers = new VkLayerProperties[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, layers);

        std::cout << "<< Layers >>" << std::endl;
        std::cout << "\tLayer Count: " << layerCount << std::endl;
        for(uint32_t i = 0; i < layerCount; i++)
        {
            std::cout << "\tlayerName: " << layers[i].layerName << std::endl;
            std::cout << "\tspecVersion: " << layers[i].specVersion << std::endl;
            std::cout << "\timplementationVersion: " << layers[i].implementationVersion << std::endl;
            std::cout << "\tdescription: " << layers[i].description << "\n" << std::endl;
        }

        std::cout << std::endl;
        delete[] layers;


        uint32_t extensionCount;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        VkExtensionProperties* extensions = new VkExtensionProperties[extensionCount];
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);

        std::cout << "<< Extensions >>" << std::endl;
        std::cout << "\tExtensionCount: " << extensionCount << std::endl;

        for(uint32_t i = 0; i < extensionCount; i++) 
        {
            std::cout << "\tName " << extensions[i].extensionName << std::endl;
            std::cout << "\tSpec Version " << extensions[i].specVersion << "\n" << std::endl;
        }

        std::cout << std::endl;
        delete[] extensions;
    }

    void updateWindowSize() {
        xcb_connection_t* connection = ZWindow::getConnection();
        xcb_window_t window = ZWindow::getWindow();
        
        xcb_get_geometry_cookie_t geomCookie = xcb_get_geometry (connection, window);
        xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply (connection, geomCookie, nullptr);

        win_width = geom->width;
        win_height = geom->height;
    }

    void update(float dt) {
        for (ZObject& obj : objects) {
            obj.update(dt);
            obj.uniform.update(0, proj, Scene::getView(), ZeroVulkan::mat4(1.f), ZeroVulkan::vec3(1.f, 1.f, 3.f));
        }
    }

    void updateProj() {
        float aspect = (float)win_width / (float)win_height;
        float fovY = 70.f;
        float farValue = 30.f;
        float nearValue = 0.001f;
        
        proj[0][0] = 1.f/ (aspect * std::tan(radian(fovY / 2.f)));
        proj[1][1] = -1.f / std::tan(radian(fovY / 2.f));// -1.f to flip y_axis
        proj[2][2] = farValue / (nearValue - farValue);
        proj[2][3] = -1.f;
        proj[3][2] = (farValue * nearValue) / (nearValue - farValue);
        proj[3][3] = 0.f;
    }

    void record() {
        for(auto& shader : ZComputeShader::computeShaders)
            shader->buildCommandBuffer();

        VkCommandBufferBeginInfo commandBufferBeginInfo = {};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        VkClearValue clearValues[2];
        clearValues[0].color = { 0.f, 0.f, 0.f, 1.f };
        clearValues[1].depthStencil = { 1.f, 0 };

        VkRenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = ZDevice::getRenderPass();
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = { win_width, win_height };
        renderPassBeginInfo.clearValueCount = 2;
        renderPassBeginInfo.pClearValues = clearValues;

        viewport.x = 0.f;
        viewport.y = 0.f;
        viewport.width = (float)win_width;
        viewport.height = (float)win_height;
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;

        scissor.offset = { 0, 0 };
        scissor.extent = { win_width, win_height };

        int i = 0;
        for(VkCommandBuffer cmdBuffer : ZDevice::getCommandPool()->getBuffers())
        {
            renderPassBeginInfo.framebuffer = ZDevice::getSwapchainFramebuffers()[i++];
            
            VkResult res = vkBeginCommandBuffer(cmdBuffer, &commandBufferBeginInfo);
            if (res != VK_SUCCESS)
                printf("beginn command buffer ERROR: %d\n", res);

            vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

            vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

            VkDeviceSize offsets[] = { 0 };

            for(const ZObject& obj : objects) {
                obj.material.bind(cmdBuffer);
                vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &obj.vertexBuffer, offsets);
                vkCmdBindIndexBuffer(cmdBuffer, obj.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
                vkCmdDrawIndexed(cmdBuffer, obj.indices_count, 1, 0, 0, 0);
            }
            vkCmdEndRenderPass(cmdBuffer);

            res = vkEndCommandBuffer(cmdBuffer);
            if(res != VK_SUCCESS)
                printf("end command buffer ERROR: %d\n", res);
        }
    }

    void initRenderer() {
        updateWindowSize();
        // default view
        Scene::setView(ZeroVulkan::vec3(-2.f, 1.f, 0.7f), ZeroVulkan::vec3(0.f, 0.f, 0.f));
        updateProj();

        createXcbSurface(ZWindow::getConnection(), ZWindow::getWindow());
        checkSurfaceSupport();
        createSwapchain(win_width, win_height);
        createRenderPass();
        createSwapchainImgViews();
        initDepthBuffering();
        createFramebuffers(ZDevice::getSwapchainExtent().width, ZDevice::getSwapchainExtent().height);
        ZDevice::createCommandPool();
        ZDevice::getCommandPool()->createCommandBuffers(static_cast<uint32_t>(ZDevice::getSwapchainImages().size()));
        createSampler();
        createSyncObjects(ZDevice::MAX_FRAMES_IN_FLIGHT);   
    }

    void refreshSwapchain()
    {
        vkDeviceWaitIdle(ZDevice::getDevice());

        vkFreeCommandBuffers(ZDevice::getDevice(), ZDevice::getCommandPool()->getPool(), (uint32_t)ZDevice::getCommandPool()->getBuffers().size(), ZDevice::getCommandPool()->getBuffers().data());

        for(uint32_t i = 0; i < ZDevice::getSwapchainImageViews().size(); i++)
            vkDestroyFramebuffer(ZDevice::getDevice(), ZDevice::getSwapchainFramebuffers()[i], nullptr);

        vkDestroyImageView(ZDevice::getDevice(), ZDevice::getDepthImageView(), nullptr);
        vkDestroyImage(ZDevice::getDevice(), ZDevice::getDepthImage(), nullptr);
        vkFreeMemory(ZDevice::getDevice(), ZDevice::getDepthImageMemory(), nullptr);

        vkDestroyRenderPass(ZDevice::getDevice(), ZDevice::getRenderPass(), nullptr);

        for(uint32_t i = 0; i < ZDevice::getSwapchainImageViews().size(); i++)
            vkDestroyImageView(ZDevice::getDevice(), ZDevice::getSwapchainImageViews()[i], nullptr);

        VkSwapchainKHR oldSwapchain = ZDevice::getSwapchain();

        updateWindowSize();
        createSwapchain(win_width, win_height);
        VkExtent2D& extent = ZDevice::getSwapchainExtent();
        updateProj();

        createSwapchainImgViews();
        createRenderPass();
        initDepthBuffering();
        createFramebuffers(extent.width, extent.height);
        ZDevice::getCommandPool()->createCommandBuffers((uint32_t) ZDevice::getSwapchainImages().size());
        record();

        vkDestroySwapchainKHR(ZDevice::getDevice(), oldSwapchain, nullptr);
    }


    void drawFrame() {
        uint32_t imgIndex;
        VkResult res;

        res = vkAcquireNextImageKHR(ZDevice::getDevice(), ZDevice::getSwapchain(), UINT64_MAX, ZDevice::getSemaphoreImgAvailable(), VK_NULL_HANDLE, &imgIndex);

        if(res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
            refreshSwapchain();

        VkPipelineStageFlags waitStageMask[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &ZDevice::getSemaphoreImgAvailable();
        submitInfo.pWaitDstStageMask = waitStageMask;
        submitInfo.commandBufferCount = 1; //watch later 
        submitInfo.pCommandBuffers = &ZDevice::getCommandPool()->getBuffers()[imgIndex];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &ZDevice::getSemaphoreRenderingDone();

        res = vkQueueSubmit(ZDevice::getQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        if(res != VK_SUCCESS)
            printf("queue submit ERROR: %d\n", res);
            
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &ZDevice::getSemaphoreRenderingDone();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &ZDevice::getSwapchain();
        presentInfo.pImageIndices = &imgIndex;

        if(vkQueuePresentKHR(ZDevice::getQueue(), &presentInfo) == VK_ERROR_OUT_OF_DATE_KHR)
            refreshSwapchain();

        for(auto& shader : ZComputeShader::computeShaders)
            shader->submit(ZDevice::getQueue());
    
        res = vkQueueWaitIdle(ZDevice::getQueue());
        if (res != VK_SUCCESS)
            printf("Queue wait idle ERROR: %d\n", res);
        
        ZDevice::getCurFrame() = (ZDevice::getCurFrame() + 1) % ZDevice::MAX_FRAMES_IN_FLIGHT;
    }
}
