#include "TurnManager.h"
#include <iostream>
#include <algorithm>

namespace miblets {

    TurnManager::TurnManager()
        : m_currentTurnIndex(0), m_currentTurnTime(0.0f), m_isMatchActive(false) {}

    void TurnManager::AddEntity(mib::ECS::Entity entity) {
        if (std::find(m_entities.begin(), m_entities.end(), entity) == m_entities.end()) {
            m_entities.push_back(entity);
        }
    }

    void TurnManager::RemoveEntity(mib::ECS::Entity entity) {
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
        if (m_currentTurnIndex >= m_entities.size() && !m_entities.empty()) {
            m_currentTurnIndex = 0;
        }
    }

    void TurnManager::StartMatch() {
        if (!m_entities.empty()) {
            m_isMatchActive = true;
            m_currentTurnIndex = 0;
            m_currentTurnTime = m_turnTimeLimit;
            std::cout << "[TurnManager] Match started. Turn: Entity " << m_entities[m_currentTurnIndex] << "\n";
        }
    }

    void TurnManager::Update(float deltaTime) {
        if (!m_isMatchActive || m_entities.empty()) return;

        m_currentTurnTime -= deltaTime;
        if (m_currentTurnTime <= 0.0f) {
            std::cout << "[TurnManager] Time up! Ending turn.\n";
            EndTurn();
        }
    }

    void TurnManager::EndTurn() {
        if (m_entities.empty()) return;

        m_currentTurnIndex = (m_currentTurnIndex + 1) % m_entities.size();
        m_currentTurnTime = m_turnTimeLimit;
        
        std::cout << "[TurnManager] Turn ended. Next turn: Entity " << m_entities[m_currentTurnIndex] << "\n";
    }

    mib::ECS::Entity TurnManager::GetCurrentEntity() const {
        if (m_entities.empty()) return 0; // Assuming 0 is invalid/null
        return m_entities[m_currentTurnIndex];
    }

    float TurnManager::GetTurnTimeRemaining() const {
        return m_currentTurnTime;
    }

}
