#ifndef VULKAN_EXCEPTION_H
#define VULKAN_EXCEPTION_H

#include <stdexcept>
#include <string>
#include <vulkan/vulkan.h>

class VulkanException : public std::runtime_error {
private:
    VkResult errorCode;

public:
    VulkanException(VkResult result, const std::string& message, const std::string& file = "", int line = 0)
        : std::runtime_error(formatMessage(result, message, file, line))
        , errorCode(result) {}

    VkResult getErrorCode() const { return errorCode; }

    static const char* getResultString(VkResult result) {
        switch (result) {
            case VK_SUCCESS: return "VK_SUCCESS";
            case VK_NOT_READY: return "VK_NOT_READY";
            case VK_TIMEOUT: return "VK_TIMEOUT";
            case VK_EVENT_SET: return "VK_EVENT_SET";
            case VK_EVENT_RESET: return "VK_EVENT_RESET";
            case VK_INCOMPLETE: return "VK_INCOMPLETE";
            case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
            case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
            case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
            case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
            case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
            case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
            case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
            case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
            case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
            case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
            case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
            case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
            case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
            default: return "UNKNOWN_VK_RESULT";
        }
    }

private:
    static std::string formatMessage(VkResult result, const std::string& message, 
                                   const std::string& file, int line) {
        std::string formatted = "Vulkan Error: " + std::string(getResultString(result)) + 
                               " (" + std::to_string(static_cast<int>(result)) + ")";
        
        if (!message.empty()) {
            formatted += "\nMessage: " + message;
        }
        
        if (!file.empty() && line > 0) {
            formatted += "\nLocation: " + file + ":" + std::to_string(line);
        }
        
        return formatted;
    }
};

// Helper macro for stringifying line numbers
#define VK_STRINGIFY(x) #x
#define VK_TOSTRING(x) VK_STRINGIFY(x)

// Main error checking macro
#define VK_CHECK(result, message) \
    do { \
        VkResult res = (result); \
        if (res != VK_SUCCESS) { \
            throw VulkanException(res, message, __FILE__, __LINE__); \
        } \
    } while(0)

// Simplified macro for common cases
#define VK_CHECK_RESULT(result) \
    VK_CHECK(result, "Vulkan operation failed")

#endif // VULKAN_EXCEPTION_H