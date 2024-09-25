/*
** EPITECH PROJECT, 2024
** rts
** File description:
** SystemManager
*/

#pragma once
#include "../../using.hpp"
#include "System.hpp"
#include <algorithm>
#include <memory>
#include <unordered_map>

class SystemManager
{
   public:
    template <typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        const char *typeName = typeid(T).name();
        auto system = std::make_shared<T>();

        mSystems.insert({typeName, system});
        return system;
    }

    template <typename T>
    std::shared_ptr<System> GetSystem()
    {
        const char *typeName = typeid(T).name();

        return mSystems[typeName];
    }

    template <typename T>
    void SetSignature(Signature signature)
    {
        const char *typeName = typeid(T).name();

        mSignatures.insert({typeName, signature});
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const &pair : mSystems) {
            auto const &system = pair.second;
            auto it = std::find(
                system->m_Entities.begin(), system->m_Entities.end(), entity);
            if (it != system->m_Entities.end())
                system->m_Entities.erase(it);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for (auto const &pair : mSystems) {
            auto const &type = pair.first;
            auto const &system = pair.second;
            auto const &systemSignature = mSignatures[type];
            auto it = std::find(
                system->m_Entities.begin(), system->m_Entities.end(), entity);
            if ((entitySignature & systemSignature) != 0) {
                if (it == system->m_Entities.end())
                    system->m_Entities.emplace_back(entity);
            } else {
                if (it != system->m_Entities.end())
                    system->m_Entities.erase(it);
            }
        }
    }

   private:
    std::unordered_map<std::string, Signature> mSignatures;
    std::unordered_map<std::string, std::shared_ptr<System>> mSystems;
};
