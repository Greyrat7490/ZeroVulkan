#include <cstdint>
#include <ctime>
#include <vector>
#include "Window/window.h"
#include "ZRenderer.h"

namespace ZeroVulkan::ZRenderer {
    VkViewport viewport;
    VkRect2D scissor;

    vec2 winSize;
    clock_t lastResize;
    bool resized = true;

    void printVulkanInfos() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        VkLayerProperties* layers = new VkLayerProperties[layerCount];
        vkEnumerateInstanceLayerProperties(&layerCount, layers);

        std::cout << "<< Layers >>" << std::endl;
        std::cout << "\tLayer Count: " << layerCount << std::endl;
        for(uint32_t i = 0; i < layerCount; i++) {
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

        for(uint32_t i = 0; i < extensionCount; i++) {
            std::cout << "\tName " << extensions[i].extensionName << std::endl;
            std::cout << "\tSpec Version " << extensions[i].specVersion << "\n" << std::endl;
        }

        std::cout << std::endl;
        delete[] extensions;
    }


    void update(float dt) {
        ZScene::current().update(dt);
        ZScene::current().postUpdate();
    }

    void start() {
        ZScene::current().start();
    }

    void end() {
        if (ZScene::getSceneCount())
            ZScene::current().end();
    }
    
    void record() {
        for (auto shader : ZComputeShader::computeShaders)
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
        renderPassBeginInfo.renderArea.extent = { (uint32_t)winSize[0], (uint32_t)winSize[1] };
        renderPassBeginInfo.clearValueCount = 2;
        renderPassBeginInfo.pClearValues = clearValues;

        viewport.x = 0.f;
        viewport.y = 0.f;
        viewport.width = winSize[0];
        viewport.height = winSize[1];
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;

        scissor.offset = { 0, 0 };
        scissor.extent = { (uint32_t)winSize[0], (uint32_t)winSize[1] };

        int i = 0;
        for (VkCommandBuffer cmdBuffer : ZDevice::getCommandPool()->getBuffers())
        {
            renderPassBeginInfo.framebuffer = ZDevice::getSwapchainFramebuffers()[i++];
            
            VkResult res = vkBeginCommandBuffer(cmdBuffer, &commandBufferBeginInfo);
            if (res != VK_SUCCESS)
                printf("beginn command buffer ERROR: %d\n", res);

            vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

            vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

            ZScene::current().bind(cmdBuffer);

            vkCmdEndRenderPass(cmdBuffer);

            res = vkEndCommandBuffer(cmdBuffer);
            if (res != VK_SUCCESS)
                printf("end command buffer ERROR: %d\n", res);
        }
    }

    void initRenderer() {
        updateWinSize();
        
        createXcbSurface(ZWindow::getConnection(), ZWindow::getWindow());
        checkSurfaceSupport();
        createSwapchain(winSize[0], winSize[1]);
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

        for (uint32_t i = 0; i < ZDevice::getSwapchainImageViews().size(); i++)
            vkDestroyFramebuffer(ZDevice::getDevice(), ZDevice::getSwapchainFramebuffers()[i], nullptr);

        vkDestroyImageView(ZDevice::getDevice(), ZDevice::getDepthImageView(), nullptr);
        vkDestroyImage(ZDevice::getDevice(), ZDevice::getDepthImage(), nullptr);
        vkFreeMemory(ZDevice::getDevice(), ZDevice::getDepthImageMemory(), nullptr);

        vkDestroyRenderPass(ZDevice::getDevice(), ZDevice::getRenderPass(), nullptr);

        for (uint32_t i = 0; i < ZDevice::getSwapchainImageViews().size(); i++)
            vkDestroyImageView(ZDevice::getDevice(), ZDevice::getSwapchainImageViews()[i], nullptr);

        VkSwapchainKHR oldSwapchain = ZDevice::getSwapchain();


        updateWinSize();
        ZScene::current().updateProj();

        createSwapchain(winSize[0], winSize[1]);
        createSwapchainImgViews();
        createRenderPass();
        initDepthBuffering();
        VkExtent2D& extent = ZDevice::getSwapchainExtent();
        createFramebuffers(extent.width, extent.height);
        ZDevice::getCommandPool()->createCommandBuffers((uint32_t) ZDevice::getSwapchainImages().size());
        record();

        vkDestroySwapchainKHR(ZDevice::getDevice(), oldSwapchain, nullptr);
    }

    void resizing() {
        lastResize = clock();
        resized = false;
    }
    
    void drawFrame() {
        uint32_t imgIndex;
        VkResult res;
        
        if (!resized) {
            // TODO is 200ms even on slower machines good
            // only refresh swapchain if the window was not resized for 200ms
            // otherwise would cause serious stuttering and even temporarily freezing of your system
            if ( ( (float)(clock() - lastResize) / CLOCKS_PER_SEC ) >= 0.2f ) {
                refreshSwapchain();
                resized = true;
            }
            
            return;
        }

        res = vkAcquireNextImageKHR(ZDevice::getDevice(), ZDevice::getSwapchain(), UINT64_MAX, ZDevice::getSemaphoreImgAvailable(), VK_NULL_HANDLE, &imgIndex);

        // resize errors should be handled at the beginning, so this should catch rather all kinds of errors
        if (res != VK_SUCCESS) {
            // not sure if still resize errors could occure so print if this happens and find out why exactly
            if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
                printf("\n\nWARNING: still resize error (TODO: find out why)\n\n");
                
            printf("AcquireNextImage ERROR: %d\n", res);
            refreshSwapchain();
            return;
        }
                
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
        if (res != VK_SUCCESS)
            printf("queue submit ERROR: %d\n", res);
            
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &ZDevice::getSemaphoreRenderingDone();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &ZDevice::getSwapchain();
        presentInfo.pImageIndices = &imgIndex;

        if (vkQueuePresentKHR(ZDevice::getQueue(), &presentInfo) == VK_ERROR_OUT_OF_DATE_KHR)
            return;

        for (auto& shader : ZComputeShader::computeShaders)
            shader->submit(ZDevice::getQueue());
    
        res = vkQueueWaitIdle(ZDevice::getQueue());
        if (res != VK_SUCCESS)
            printf("Queue wait idle ERROR: %d\n", res);
        
        ZDevice::getCurFrame() = (ZDevice::getCurFrame() + 1) % ZDevice::MAX_FRAMES_IN_FLIGHT;
    }

    void updateWinSize() {
        winSize = ZWindow::getSize();
    }
 
    void clear() {
        vkDeviceWaitIdle(ZDevice::getDevice());
        
        ZScene::clear();
        ZDevice::clear();
    }
}
