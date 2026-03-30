#include "MibletsApp.h"
#include "Engine/ECS/Components.h"
#include "Components.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace miblets {

    MibletsApp::MibletsApp() {
        m_engine = std::make_unique<mib::Engine>();
        m_registry = std::make_unique<mib::ECS::Registry>();
        m_turnManager = std::make_unique<TurnManager>();
    }

    MibletsApp::~MibletsApp() {
        Shutdown();
    }

    bool MibletsApp::Initialize() {
        if (!m_engine->Initialize()) {
            return false;
        }

        // We do NOT call SetupLevel() here anymore.
        // It gets called dynamically when transitioning out of the Main Menu!

        // Bind game loop mechanics (Worms-style turned-based mechanics setup here)
        m_engine->SetGameUpdateCallback([this](float dt) {
            this->Update(dt);
        });

        return true;
    }

    void MibletsApp::SetupLevel() {
        std::cout << "Setting up Miblets Level...\n";
        
        m_registry->RegisterComponent<mib::ECS::TransformComponent>();
        m_registry->RegisterComponent<mib::ECS::MeshComponent>();
        m_registry->RegisterComponent<mib::ECS::MibletComponent>();
        m_registry->RegisterComponent<miblets::ProjectileComponent>();
        m_registry->RegisterComponent<miblets::WeaponComponent>();

        // Spawn a main character Miblet
        mib::ECS::Entity player1 = m_registry->CreateEntity();
        
        m_registry->AddComponent(player1, mib::ECS::TransformComponent{
            {0.0f, 10.0f, 0.0f}, // position
            {0.0f, 0.0f, 0.0f},  // rotation
            {1.0f, 1.0f, 1.0f}   // scale
        });
        
        m_registry->AddComponent(player1, mib::ECS::MeshComponent{
            "Assets/Models/MibletBase.obj",
            "BlueTeamSkin"
        });

        m_registry->AddComponent(player1, mib::ECS::MibletComponent{
            "Sergeant Miblet",
            100.0f
        });

        m_turnManager->AddEntity(player1);
        
        mib::ECS::Entity player2 = m_registry->CreateEntity();
        m_registry->AddComponent(player2, mib::ECS::TransformComponent{
            {10.0f, 10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}
        });
        m_registry->AddComponent(player2, mib::ECS::MeshComponent{
            "Assets/Models/MibletBase.obj", "RedTeamSkin"
        });
        m_registry->AddComponent(player2, mib::ECS::MibletComponent{
            "Private Miblet", 100.0f
        });
        m_registry->AddComponent(player2, miblets::WeaponComponent{});

        m_turnManager->AddEntity(player2);
        m_turnManager->StartMatch();

        std::cout << "Level initialized with " << m_registry->GetComponent<mib::ECS::MibletComponent>(player1).Name << "\n";
    }

    void MibletsApp::DrawMainMenu() {
        static bool enterPressedLast = false;
        static bool oPressedLast = false;
        
        bool enterPressed = m_engine->IsKeyPressed(GLFW_KEY_ENTER);
        bool oPressed = m_engine->IsKeyPressed(GLFW_KEY_O);
        
        // Use a static timer so we don't spam the console too fast
        static float menuRefresh = 0.0f;
        menuRefresh += 0.016f; // rough estimate for display
        if (menuRefresh > 1.0f) {
            std::cout << "\n===============================\n"
                      << "           MIBLETS           \n"
                      << "===============================\n"
                      << " [ENTER]  Start Game\n"
                      << " [O]      Options\n"
                      << "===============================\n";
            menuRefresh = 0.0f;
        }

        if (enterPressed && !enterPressedLast) {
            std::cout << "\n[UI] Transitioning to Game!\n";
            m_currentState = GameState::Playing;
            SetupLevel(); // Reset/Start level
        }
        if (oPressed && !oPressedLast) {
            std::cout << "\n[UI] Transitioning to Options!\n";
            m_currentState = GameState::Options;
        }

        enterPressedLast = enterPressed;
        oPressedLast = oPressed;
    }

    void MibletsApp::DrawOptionsMenu() {
        static bool escPressedLast = false;
        static bool numPressedLast[3] = {false, false, false};
        
        bool escPressed = m_engine->IsKeyPressed(GLFW_KEY_ESCAPE);
        bool n1 = m_engine->IsKeyPressed(GLFW_KEY_1);
        bool n2 = m_engine->IsKeyPressed(GLFW_KEY_2);
        bool n3 = m_engine->IsKeyPressed(GLFW_KEY_3);

        if (n1 && !numPressedLast[0]) m_optionsTab = 0;
        if (n2 && !numPressedLast[1]) m_optionsTab = 1;
        if (n3 && !numPressedLast[2]) m_optionsTab = 2;

        static float menuRefresh = 0.0f;
        menuRefresh += 0.016f;
        if (menuRefresh > 1.0f) {
            std::cout << "\n=========== OPTIONS ===========\n"
                      << " Tabs: [1] Video | [2] Audio | [3] Gameplay \n\n";
            
            if (m_optionsTab == 0) {
                std::cout << " -- VIDEO SETTINGS --\n"
                          << " * Resolution: 1920x1080\n"
                          << " * Fullscreen: ON\n"
                          << " * V-Sync: ON\n";
            } else if (m_optionsTab == 1) {
                std::cout << " -- AUDIO SETTINGS --\n"
                          << " * Master Volume: 80%\n"
                          << " * Effects Volume: 100%\n";
            } else if (m_optionsTab == 2) {
                std::cout << " -- GAMEPLAY SETTINGS --\n"
                          << " * Turn Time Limit: 30 Seconds\n";
            }
            std::cout << "\n [ESC] Back to Main Menu\n"
                      << "===============================\n";
            menuRefresh = 0.0f;
        }

        if (escPressed && !escPressedLast) {
            std::cout << "\n[UI] Transitioning to Main Menu!\n";
            m_currentState = GameState::MainMenu;
        }

        escPressedLast = escPressed;
        numPressedLast[0] = n1;
        numPressedLast[1] = n2;
        numPressedLast[2] = n3;
    }

    void MibletsApp::Update(float deltaTime) {
        std::vector<glm::mat4> models;

        if (m_currentState == GameState::MainMenu) {
            DrawMainMenu();
            m_engine->SetRenderables(models); // render empty screen
            return;
        }
        else if (m_currentState == GameState::Options) {
            DrawOptionsMenu();
            m_engine->SetRenderables(models); // render empty screen
            return;
        }

        // Static floor
        glm::mat4 floorModel = glm::mat4(1.0f);
        floorModel = glm::translate(floorModel, glm::vec3(0.0f, -1.0f, 0.0f));
        floorModel = glm::scale(floorModel, glm::vec3(50.0f, 1.0f, 50.0f));
        models.push_back(floorModel);

        m_gameTime += deltaTime;
        m_turnManager->Update(deltaTime);
        
        static float printTimer = 0.0f;
        printTimer += deltaTime;

        // Input Polling for Shooting (Phase 8 Polish)
        static bool spacePressedLastFrame = false;
        static bool tabPressedLastFrame = false;
        bool spacePressed = m_engine->IsKeyPressed(GLFW_KEY_SPACE);
        bool tabPressed = m_engine->IsKeyPressed(GLFW_KEY_TAB);
        bool leftPressed = m_engine->IsKeyPressed(GLFW_KEY_A);
        bool rightPressed = m_engine->IsKeyPressed(GLFW_KEY_D);

        mib::ECS::Entity currentEntity = m_turnManager->GetCurrentEntity();
        if (m_registry->HasComponent<mib::ECS::TransformComponent>(currentEntity)) {
            auto& activeTransform = m_registry->GetComponent<mib::ECS::TransformComponent>(currentEntity);
            auto& activeMiblet = m_registry->GetComponent<mib::ECS::MibletComponent>(currentEntity);
            
            // Movement logic
            float moveSpeed = 5.0f * deltaTime;
            if (leftPressed) {
                activeTransform.Position.x -= moveSpeed;
            }
            if (rightPressed) {
                activeTransform.Position.x += moveSpeed;
            }

            // Weapon switching logic
            static int weaponIndex = 0;
            const char* weapons[] = {"Bazooka", "Grenade"};
            if (tabPressed && !tabPressedLastFrame) {
                weaponIndex = (weaponIndex + 1) % 2;
            }
            tabPressedLastFrame = tabPressed;
            
            // HUD display on the same line
            std::cout << "\rHUD Menu [HP:" << activeMiblet.Health 
                      << " | Inventory: " << weapons[weaponIndex] 
                      << " | Active: " << activeMiblet.Name << "]               " << std::flush;

            // Firing logic
            if (spacePressed && !spacePressedLastFrame) {
                std::cout << "\n[Gameplay] FIRE! " << weapons[weaponIndex] << " fired from " 
                          << activeMiblet.Name << "!\n";

                // Spawn a Projectile
                mib::ECS::Entity projEntity = m_registry->CreateEntity();
                m_registry->AddComponent(projEntity, mib::ECS::TransformComponent{
                    {activeTransform.Position[0], activeTransform.Position[1] + 1.0f, activeTransform.Position[2]}, // Start slightly above
                    {0.0f, 0.0f, 0.0f},
                    {0.2f, 0.2f, 0.2f}
                });
                float fireVelocity = (weaponIndex == 0) ? 15.0f : 5.0f; // Bazooka vs Grenade logic
                m_registry->AddComponent(projEntity, miblets::ProjectileComponent{
                    {5.0f, fireVelocity, 0.0f}, // Fire arc (up and forward)
                    5.0f,
                    true
                });
            }
        }
        spacePressedLastFrame = spacePressed;

        // Phase 5 & 6 Gameplay Loop:
        // Update all Miblets via ECS
        auto entities = m_registry->View<mib::ECS::TransformComponent, mib::ECS::MibletComponent>();
        
        for (auto e : entities) {
            auto& transform = m_registry->GetComponent<mib::ECS::TransformComponent>(e);
            auto& miblet = m_registry->GetComponent<mib::ECS::MibletComponent>(e);

            // Simple physics simulation (gravity / bouncing) for demo
            transform.Position.y -= 9.8f * deltaTime; // Gravity
            if (transform.Position.y < 0.0f) {
                transform.Position.y = 10.0f; // Bounce back up
            }

            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, transform.Position);
            // Ignore rotation for simplicity in this pass, or add if wanted
            modelMat = glm::scale(modelMat, transform.Scale);
            models.push_back(modelMat);

            if (printTimer >= 1.0f && m_turnManager->GetCurrentEntity() == e) {
                std::cout << "[Gameplay] " << miblet.Name 
                          << " (Active Turn) is at Y: " << transform.Position.y 
                          << ", Time Left: " << m_turnManager->GetTurnTimeRemaining() << "\n";
            }
        }

        // Phase 6 Projectile Physics
        auto projectiles = m_registry->View<mib::ECS::TransformComponent, miblets::ProjectileComponent>();
        for (auto e : projectiles) {
            auto& transform = m_registry->GetComponent<mib::ECS::TransformComponent>(e);
            auto& proj = m_registry->GetComponent<miblets::ProjectileComponent>(e);

            if (!proj.Active) continue;

            proj.Velocity.y -= 9.81f * deltaTime; // Gravity parabola
            transform.Position += proj.Velocity * deltaTime;

            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, transform.Position);
            modelMat = glm::scale(modelMat, transform.Scale);
            models.push_back(modelMat);

            // Explosion logic when crossing y < 0
            if (transform.Position.y < 0.0f) {
                std::cout << "[Gameplay] BOOM! Projectile exploded at (" 
                          << transform.Position.x << ", 0.0, " << transform.Position.z << ") with radius " 
                          << proj.DamageRadius << "\n";
                proj.Active = false;
                
                // End turn after projectile explodes (Worms style)
                m_turnManager->EndTurn();
                
                // Optional: We could destroy the entity here.
                // m_registry->DestroyEntity(e);
            }
        }

        m_engine->SetRenderables(models);

        if (printTimer >= 1.0f) {
            printTimer = 0.0f;
        }
    }

    void MibletsApp::Run() {
        m_engine->Run();
    }

    void MibletsApp::Shutdown() {
        if (m_engine) {
            m_engine->Shutdown();
        }
    }

}