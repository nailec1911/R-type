/*
** EPITECH PROJECT, 2024
** rts
** File description:
** SystemManager
*/

#pragma once
#include "using.hpp"
#include "System.hpp"

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
    void SetSignature(Signature signature)
    {
        const char *typeName = typeid(T).name();

        mSignatures.insert({typeName, signature});
    }

    void EntityDestroyed(Entity entity)
    {
        for (auto const &pair : mSystems)
        {
            auto const &system = pair.second;

            system->mEntities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for (auto const &pair : mSystems)
        {
            auto const &type = pair.first;
            auto const &system = pair.second;
            auto const &systemSignature = mSignatures[type];

            if ((entitySignature & systemSignature) == systemSignature)
                system->mEntities.insert(entity);
            else
                system->mEntities.erase(entity);
        }
    }

private:
    std::unordered_map<const char *, Signature> mSignatures{};
    std::unordered_map<const char *, std::shared_ptr<System>> mSystems{};
};
