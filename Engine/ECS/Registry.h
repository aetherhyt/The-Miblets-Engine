#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include "Components.h"

namespace mib::ECS
{
    using Entity = uint32_t;

    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        void InsertData(Entity entity, T component)
        {
            m_ComponentMap[entity] = component;
        }

        void RemoveData(Entity entity)
        {
            m_ComponentMap.erase(entity);
        }

        T& GetData(Entity entity)
        {
            return m_ComponentMap.at(entity);
        }

        bool HasData(Entity entity) const
        {
            return m_ComponentMap.find(entity) != m_ComponentMap.end();
        }

        void EntityDestroyed(Entity entity) override
        {
            if (HasData(entity))
            {
                RemoveData(entity);
            }
        }

    private:
        std::unordered_map<Entity, T> m_ComponentMap;
    };

    class Registry
    {
    public:
        Entity CreateEntity()
        {
            return ++m_NextEntity;
        }

        void DestroyEntity(Entity entity)
        {
            for (auto const& pair : m_ComponentArrays)
            {
                pair.second->EntityDestroyed(entity);
            }
        }

        template <typename T>
        void RegisterComponent()
        {
            std::type_index typeName = typeid(T);
            m_ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
        }

        template <typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->InsertData(entity, component);
        }

        template <typename T>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<T>()->RemoveData(entity);
        }

        template <typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->GetData(entity);
        }

        template <typename T>
        bool HasComponent(Entity entity)
        {
            return GetComponentArray<T>()->HasData(entity);
        }

        template <typename... Ts>
        std::vector<Entity> View()
        {
            std::vector<Entity> result;
            for (Entity i = 1; i <= m_NextEntity; ++i)
            {
                if ((HasComponent<Ts>(i) && ...))
                {
                    result.push_back(i);
                }
            }
            return result;
        }

    private:
        Entity m_NextEntity = 0;
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays;

        template <typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            std::type_index typeName = typeid(T);
            return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
        }
    };
}
