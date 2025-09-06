#ifndef VULKAN_WRAPPER_H
#define VULKAN_WRAPPER_H

#include <vulkan/vulkan.h>
#include <memory>
#include <functional>
#include "VulkanException.h"

namespace VulkanRAII {

// Custom deleter template for Vulkan objects
template<typename T>
struct VulkanDeleter {
    void operator()(T object) const = delete; // Force specialization
};

// Specialized deleters for different Vulkan objects
template<>
struct VulkanDeleter<VkInstance> {
    void operator()(VkInstance instance) const {
        if (instance != VK_NULL_HANDLE) {
            vkDestroyInstance(instance, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkDevice> {
    void operator()(VkDevice device) const {
        if (device != VK_NULL_HANDLE) {
            vkDestroyDevice(device, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkBuffer> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkBuffer buffer) const {
        if (buffer != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, buffer, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkDeviceMemory> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkDeviceMemory memory) const {
        if (memory != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkFreeMemory(device, memory, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkImageView> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkImageView imageView) const {
        if (imageView != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyImageView(device, imageView, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkShaderModule> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkShaderModule shaderModule) const {
        if (shaderModule != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device, shaderModule, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkPipeline> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkPipeline pipeline) const {
        if (pipeline != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyPipeline(device, pipeline, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkPipelineLayout> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkPipelineLayout pipelineLayout) const {
        if (pipelineLayout != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkRenderPass> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkRenderPass renderPass) const {
        if (renderPass != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device, renderPass, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkFramebuffer> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkFramebuffer framebuffer) const {
        if (framebuffer != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkCommandPool> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkCommandPool commandPool) const {
        if (commandPool != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device, commandPool, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkSemaphore> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkSemaphore semaphore) const {
        if (semaphore != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroySemaphore(device, semaphore, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkFence> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkFence fence) const {
        if (fence != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyFence(device, fence, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkDescriptorPool> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkDescriptorPool descriptorPool) const {
        if (descriptorPool != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkDescriptorSetLayout> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkDescriptorSetLayout descriptorSetLayout) const {
        if (descriptorSetLayout != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkSwapchainKHR> {
    VkDevice device;
    explicit VulkanDeleter(VkDevice dev) : device(dev) {}
    
    void operator()(VkSwapchainKHR swapchain) const {
        if (swapchain != VK_NULL_HANDLE && device != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(device, swapchain, nullptr);
        }
    }
};

template<>
struct VulkanDeleter<VkSurfaceKHR> {
    VkInstance instance;
    explicit VulkanDeleter(VkInstance inst) : instance(inst) {}
    
    void operator()(VkSurfaceKHR surface) const {
        if (surface != VK_NULL_HANDLE && instance != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
        }
    }
};

// Type aliases for unique_ptr with custom deleters
using UniqueInstance = std::unique_ptr<std::remove_pointer_t<VkInstance>, VulkanDeleter<VkInstance>>;
using UniqueDevice = std::unique_ptr<std::remove_pointer_t<VkDevice>, VulkanDeleter<VkDevice>>;

// Helper functions to create unique handles
template<typename T>
auto makeUnique(T handle, VkDevice device) {
    return std::unique_ptr<std::remove_pointer_t<T>, VulkanDeleter<T>>(
        handle, VulkanDeleter<T>(device)
    );
}

template<typename T>
auto makeUnique(T handle, VkInstance instance) {
    return std::unique_ptr<std::remove_pointer_t<T>, VulkanDeleter<T>>(
        handle, VulkanDeleter<T>(instance)
    );
}

// Specializations for handles that don't need device/instance
template<>
inline auto makeUnique<VkInstance>(VkInstance handle, VkDevice) {
    return std::unique_ptr<std::remove_pointer_t<VkInstance>, VulkanDeleter<VkInstance>>(
        handle, VulkanDeleter<VkInstance>{}
    );
}

template<>
inline auto makeUnique<VkDevice>(VkDevice handle, VkInstance) {
    return std::unique_ptr<std::remove_pointer_t<VkDevice>, VulkanDeleter<VkDevice>>(
        handle, VulkanDeleter<VkDevice>{}
    );
}

// Buffer class that manages both VkBuffer and VkDeviceMemory together
class Buffer {
public:
    Buffer() = default;
    Buffer(VkDevice device, VkBuffer buffer, VkDeviceMemory memory)
        : device_(device)
        , buffer_(buffer, VulkanDeleter<VkBuffer>(device))
        , memory_(memory, VulkanDeleter<VkDeviceMemory>(device)) {}

    Buffer(Buffer&& other) noexcept = default;
    Buffer& operator=(Buffer&& other) noexcept = default;
    
    // Non-copyable
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    [[nodiscard]] VkBuffer get() const { return buffer_.get(); }
    [[nodiscard]] VkDeviceMemory getMemory() const { return memory_.get(); }
    [[nodiscard]] bool isValid() const { return buffer_ && memory_; }

    void reset() {
        buffer_.reset();
        memory_.reset();
    }

private:
    VkDevice device_ = VK_NULL_HANDLE;
    std::unique_ptr<std::remove_pointer_t<VkBuffer>, VulkanDeleter<VkBuffer>> buffer_;
    std::unique_ptr<std::remove_pointer_t<VkDeviceMemory>, VulkanDeleter<VkDeviceMemory>> memory_;
};

} // namespace VulkanRAII

#endif // VULKAN_WRAPPER_H