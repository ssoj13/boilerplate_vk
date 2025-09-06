#ifndef VULKAN_APP_H
#define VULKAN_APP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <optional>
#include <chrono>

#include "Mesh.h"
#include "VulkanException.h"

// Enable validation layers in debug builds
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat3 normalMatrix;
};

struct LightingBufferObject {
    glm::vec3 lightPos;
    glm::vec3 viewPos;
    glm::vec3 lightColor;
    float ambientStrength;
    float specularStrength;
};

class VulkanApp {
public:
    void run();

private:
    // Window settings
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const char* WINDOW_TITLE = "Vulkan App";
    bool framebufferResized = false;

    // Validation layers
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    
    // Vulkan components
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    // Swap chain
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    // Pipeline
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;

    // Framebuffers
    std::vector<VkFramebuffer> swapChainFramebuffers;

    // Command pool and buffers
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    // Sync objects
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t currentFrame = 0;
    
    // Per-frame synchronization (for CPU-GPU sync)
    std::vector<VkFence> inFlightFences;
    
    // Per-image synchronization (for proper swapchain image handling)
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    // Cube mesh
    Mesh cubeMesh;

    // Uniform buffers
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VkBuffer> lightingBuffers;
    std::vector<VkDeviceMemory> lightingBuffersMemory;

    // Descriptor sets
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;

    // Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Functions
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    // Vulkan setup functions
    void createInstance();
    void setupDebugMessenger();
    
    // Debug helper functions
    bool checkValidationLayerSupport() const;
    std::vector<const char*> getRequiredExtensions() const;
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    VkResult createDebugUtilsMessengerEXT(VkInstance inst, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, 
        VkDebugUtilsMessengerEXT* pDebugMessenger);
    void destroyDebugUtilsMessengerEXT(VkInstance inst, 
        VkDebugUtilsMessengerEXT debugMsg, 
        const VkAllocationCallbacks* pAllocator);
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createCubeMesh();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();

    // Draw and update functions
    void drawFrame();
    void updateUniformBuffer(uint32_t currentImage);

    // Helper functions
    std::vector<char> readFile(const std::string& filename);
    bool isDeviceSuitable(VkPhysicalDevice physicalDev);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDev);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDev);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDev);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule createShaderModule(const std::vector<char>& code);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void recreateSwapChain();
    void cleanupSwapChain();
};

#endif // VULKAN_APP_H