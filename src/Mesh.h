#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    void createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, 
                          VkQueue graphicsQueue, VkCommandPool commandPool);
    void createIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                         VkQueue graphicsQueue, VkCommandPool commandPool);
    void cleanup(VkDevice device);
};

class MeshGenerator {
public:
    static Mesh generateCube(float width, float height, float depth);
    static Mesh generatePlane(float width, float height);
    // Other mesh generation functions can be added here
};

#endif // MESH_H