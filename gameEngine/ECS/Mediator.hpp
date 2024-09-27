/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Mediator
*/

#pragma once
#include "Managers/Component/ComponentManager.hpp"
#include "Managers/Entity/EntityManager.hpp"
#include "Managers/System/SystemManager.hpp"
#include "using.hpp"
#include "../Renderer/Elements.hpp"

class Mediator
{
   public:
    Mediator()
    {
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        mSystemManager = std::make_unique<SystemManager>();
    }

    Entity CreateEntity()
    {
        return mEntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity)
    {
        mEntityManager->DestroyEntity(entity);
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
    }

    template <typename T>
    void RegisterComponent()
    {
        mComponentManager->RegisterComponent<T>();
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        mComponentManager->AddComponent<T>(entity, component);

        auto signature = mEntityManager->GetSignature(entity);

        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    void RemoveComponent(Entity entity)
    {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->GetSignature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->SetSignature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    T &GetComponent(Entity entity)
    {
        return mComponentManager->GetComponent<T>(entity);
    }

    template <typename T>
    ComponentType GetComponentType()
    {
        return mComponentManager->GetComponentType<T>();
    }

    template <typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return mSystemManager->RegisterSystem<T>();
    }

    template <typename T>
    void SetSystemSignature(Signature signature)
    {
        mSystemManager->SetSignature<T>(signature);
    }

    template <typename T>
    std::shared_ptr<System> GetSystem()
    {
        return mSystemManager->GetSystem<T>();
    }

    elementTypes GetEntityRole(Entity entity)
    {
        if (mEntityManager->GetSignature(entity).test(PLAYER))
            return elementTypes::PLAYER1;
        if (mEntityManager->GetSignature(entity).test(WALL))
            return elementTypes::WALL_1;
        if (mEntityManager->GetSignature(entity).test(MONSTER))
            return elementTypes::RED_MONSTER_LEFT;
        if (mEntityManager->GetSignature(entity).test(BULLET))
            return elementTypes::BULLET_P;
        if (mEntityManager->GetSignature(entity).test(HUD))
            return elementTypes::BONUS_ITEM;
        return elementTypes::NONE;
    }

    std::unordered_map<Entity, Signature> GetEntitiesSignatures()
    {
        return this->mEntityManager->GetEntitiesSignatures();
    }

   private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<SystemManager> mSystemManager;
};
