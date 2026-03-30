#include "Renderer.h"
#include "Engine/RHI/Vulkan/VulkanContext.h"

namespace mib
{
    Renderer::Renderer(VulkanContext& context)
        : m_context(context)
    {
    }

    bool Renderer::Initialize()
    {
        return true;
    }

    void Renderer::RenderFrame(const std::vector<glm::mat4>& models)
    {
        m_context.DrawFrame(models);
    }

    void Renderer::Shutdown()
    {
    }
}
