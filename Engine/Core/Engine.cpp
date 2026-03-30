#include "Engine.h"
#include <iostream>
#include <chrono>
#include "Engine/Platform/GLFWWindow.h"
#include "Engine/RHI/Vulkan/VulkanContext.h"
#include "Engine/Renderer/Renderer.h"

namespace mib
{
    Engine::Engine() = default;
    Engine::~Engine() = default;

    // ...
    
    void Engine::Quit()
    {
        m_running = false;
    }

    bool Engine::IsKeyPressed(int key) const
    {
        return m_window ? m_window->IsKeyPressed(key) : false;
    }

    bool Engine::Initialize()
    {
        std::cout << "Initializing Miblets Engine (Phase 1)...\n";

        m_window = std::make_unique<GLFWWindow>();
        if (!m_window->Initialize(1600, 900, "Miblets Engine - Basic Triangle"))
        {
            std::cerr << "Failed to initialize GLFW Window\n";
            return false;
        }

        m_vulkan = std::make_unique<VulkanContext>();
        if (!m_vulkan->Initialize(*m_window))
        {
            std::cerr << "Failed to initialize Vulkan Context\n";
            return false;
        }

        m_renderer = std::make_unique<Renderer>(*m_vulkan);
        if (!m_renderer->Initialize())
        {
            std::cerr << "Failed to initialize Renderer\n";
            return false;
        }

        m_uiSystem = std::make_unique<UI::NoesisSystem>();
        m_uiSystem->Initialize(m_vulkan.get());
        m_uiSystem->LoadHUD("MainMenu.xaml");

        m_running = true;
        return true;
    }

    void Engine::Run()
    {
        std::cout << "Starting Engine Loop...\n";

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (m_running && !m_window->ShouldClose())
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
            lastTime = currentTime;

            m_window->PollEvents();

            if (m_gameUpdateCallback)
            {
                m_gameUpdateCallback(deltaTime);
            }
            
            m_uiSystem->Update(deltaTime);

            // For now, delegate the frame rendering to the renderer (which uses VulkanContext)
            m_renderer->RenderFrame(m_renderables);

            m_uiSystem->Render();
        }
    }

    void Engine::Shutdown()
    {
        std::cout << "Shutting down Miblets Engine...\n";

        if (m_vulkan)
        {
            m_vulkan->WaitIdle();
        }

        if (m_renderer)
        {
            m_renderer->Shutdown();
        }

        if (m_vulkan)
        {
            m_vulkan->Shutdown();
        }

        if (m_window)
        {
            m_window->Shutdown();
        }
    }
}

