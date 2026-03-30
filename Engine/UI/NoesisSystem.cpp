#include "NoesisSystem.h"
#include <iostream>

#include "../RHI/Vulkan/VulkanContext.h"

#ifdef USING_NOESIS
#include <NsCore/Noesis.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsRender/RenderDevice.h>
#include <NsRender/VKFactory.h>
#include <NsApp/ThemeProviders.h>
#endif

namespace mib {
    namespace UI {

        NoesisSystem::NoesisSystem() : m_vulkanContext(nullptr), m_isInitialized(false)
        {
        }

        NoesisSystem::~NoesisSystem()
        {
            Shutdown();
        }

        bool NoesisSystem::Initialize(VulkanContext* vulkanContext)
        {
            m_vulkanContext = vulkanContext;
            std::cout << "[UI] Initializing NoesisGUI System...\n";

#ifdef USING_NOESIS
            // 1. Initialize Noesis Engine
            Noesis::GUI::Init();

            // 2. Set up Theme Providers
            NoesisApp::SetThemeProviders();
            Noesis::GUI::LoadApplicationResources("Theme/NoesisTheme.DarkBlue.xaml");

            // 3. Create the Vulkan Render Device using context bridges
            Noesis::Ptr<Noesis::RenderDevice> device = NoesisApp::VKFactory::CreateDevice(
                false, // sRGB
                m_vulkanContext->GetInstance(),
                m_vulkanContext->GetPhysicalDevice(),
                m_vulkanContext->GetDevice(),
                m_vulkanContext->GetGraphicsQueue(),
                m_vulkanContext->GetGraphicsQueueFamily()
            );

            // Optional: Pass context info for offscreen rendering / depth formats 
            // VKFactory ensures Vulkan pipelines match engine constraints.
#endif

            m_isInitialized = true;
            return true;
        }

        void NoesisSystem::LoadHUD(const std::string& xamlPath)
        {
            if (!m_isInitialized) return;
            std::cout << "[UI] Loading XAML view from " << xamlPath << "...\n";
            
#ifdef USING_NOESIS
            // Initialize View with parsed Resource object
            Noesis::Ptr<Noesis::FrameworkElement> xaml = 
                Noesis::GUI::LoadXaml<Noesis::FrameworkElement>(xamlPath.c_str());
            
            m_view = Noesis::GUI::CreateView(xaml);
            // Default Size
            m_view->SetSize(1600, 900);
#endif
        }

        void NoesisSystem::Update(float deltaTime)
        {
            if (!m_isInitialized) return;

#ifdef USING_NOESIS
            if (m_view) {
                // Typical Engine -> Noesis inputs:
                // m_view->MouseMove(x, y);
                // m_view->MouseButtonDown(x, y, Noesis::MouseButton_Left);
                
                m_view->Update(deltaTime);
            }
#endif
        }

        void NoesisSystem::Render()
        {
            if (!m_isInitialized) return;

#ifdef USING_NOESIS
            if (m_view) {
                // Must be executed INSIDE your VK RenderPass
                m_view->GetRenderer()->UpdateRenderTree();
                m_view->GetRenderer()->RenderOffscreen(); // Renders masks/brushes
                // m_view->GetRenderer()->Render();       // Renders directly into current active cmd buffer
            }
#endif
        }

        void NoesisSystem::Shutdown()
        {
            if (m_isInitialized) {
                std::cout << "[UI] Shutting down NoesisGUI System...\n";
#ifdef USING_NOESIS
                m_view.Reset();
                Noesis::GUI::Shutdown();
#endif
                m_isInitialized = false;
            }
        }

    } // namespace UI
} // namespace mib