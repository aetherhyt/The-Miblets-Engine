#pragma once

#include <cstddef>
#include <optional>
#include <vector>
#include <array>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace mib
{
    class GLFWWindow;

    class VulkanContext
    {
    public:
        struct Vertex
        {
            glm::vec3 pos;
            glm::vec3 color;

            static VkVertexInputBindingDescription getBindingDescription()
            {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
            {
                std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, pos);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, color);

                return attributeDescriptions;
            }
        };

        bool Initialize(GLFWWindow& window);
        void DrawFrame(const std::vector<glm::mat4>& models);
        void WaitIdle();
        void Shutdown();

    private:
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool IsComplete() const
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities{};
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        void CreateInstance();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateDepthResources();
        VkFormat FindDepthFormat() const;
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void CreateCommandBuffers();
        void CreateSyncObjects();

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        void CleanupSwapChain();
        void RecreateSwapChain();

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
        bool IsDeviceSuitable(VkPhysicalDevice device) const;
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

        VkShaderModule CreateShaderModule(const std::vector<char>& code);

    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        VkInstance GetInstance() const { return m_instance; }
        VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
        VkDevice GetDevice() const { return m_device; }
        VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
        uint32_t GetGraphicsQueueFamily() const { return FindQueueFamilies(m_physicalDevice).graphicsFamily.value(); }
        VkFormat GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
        VkFormat GetDepthFormat() const { return FindDepthFormat(); }

    private:
        GLFWWindow* m_window = nullptr;

        VkInstance m_instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;

        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
        VkExtent2D m_swapChainExtent{};

        VkImage m_depthImage = VK_NULL_HANDLE;
        VkDeviceMemory m_depthImageMemory = VK_NULL_HANDLE;
        VkImageView m_depthImageView = VK_NULL_HANDLE;

        VkRenderPass m_renderPass = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
        VkBuffer m_indexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;
        
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_commandBuffers;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;

        size_t m_currentFrame = 0;

        const std::vector<const char*> m_deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };
}
