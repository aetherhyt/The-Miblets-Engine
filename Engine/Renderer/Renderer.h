#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace mib
{
    class VulkanContext;

    class Renderer
    {
    public:
        explicit Renderer(VulkanContext& context);

        bool Initialize();
        void RenderFrame(const std::vector<glm::mat4>& models);
        void Shutdown();

    private:
        VulkanContext& m_context;
    };
}
