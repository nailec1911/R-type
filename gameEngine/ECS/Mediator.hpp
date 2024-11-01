/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Mediator
*/

#pragma once
#include <cstdint>

#include "../Renderer/Sprites.hpp"
#include "Managers/Component/ComponentManager.hpp"
#include "Managers/Entity/EntityManager.hpp"
#include "Managers/System/SystemManager.hpp"
#include "using.hpp"

class Mediator
{
   public:
    Mediator()
    {
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        mSystemManager = std::make_unique<SystemManager>();
    }

    Mediator(const Mediator &) = delete;
    Mediator(Mediator &&) = delete;
    Mediator &operator=(const Mediator &) = delete;
    Mediator &operator=(Mediator &&) = delete;
    ~Mediator() = default;

    Entity CreateEntity()
    {
        return mEntityManager->CreateEntity();
    }

    Entity CreateEntityById(Entity id)
    {
        return mEntityManager->CreateEntityById(id);
    }

    void DestroyEntity(Entity entity)
    {
        mEntityManager->DestroyEntity(entity);
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
    }

    template <typename T>
    void RegisterComponent(uint32_t bitPos)
    {
        mComponentManager->RegisterComponent<T>(bitPos);
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
    std::shared_ptr<ISystem> GetSystem()
    {
        return mSystemManager->GetSystem<T>();
    }

    spritesTypes GetEntitySprite(Entity entity)
    {
        if (mEntityManager->GetSignature(entity).test(PLAYER))
            return spritesTypes::PLAYER1;
        if (mEntityManager->GetSignature(entity).test(WALL))
            return spritesTypes::WALL_1;
        if (mEntityManager->GetSignature(entity).test(SHOOTER_MONSTER))
            return spritesTypes::BROWN_MONSTER_LEFT;
        if (mEntityManager->GetSignature(entity).test(FLYING_MONSTER))
            return spritesTypes::RED_MONSTER_LEFT;
        if (mEntityManager->GetSignature(entity).test(MONSTER))
            return spritesTypes::RED_MONSTER_LEFT;
        if (mEntityManager->GetSignature(entity).test(P_BULLET))
            return spritesTypes::BULLET_P;
        if (mEntityManager->GetSignature(entity).test(P_BULLET_CHARGED))
            return spritesTypes::BULLET_P_CHARGED_100;
        if (mEntityManager->GetSignature(entity).test(M_BULLET))
            return spritesTypes::BULLET_M;
        if (mEntityManager->GetSignature(entity).test(BONUS_SHOOT))
            return spritesTypes::BONUS_SHOOTER;
        if (mEntityManager->GetSignature(entity).test(BONUS_SPEED))
            return spritesTypes::BONUS_SPEEDY;
        if (mEntityManager->GetSignature(entity).test(HUD))
            return spritesTypes::BONUS_ITEM;
        return spritesTypes::EMPTY;
    }

    EntityName GetEntityRole(Entity entity)
    {
        if (mEntityManager->GetSignature(entity).test(PLAYER))
            return PLAYER;
        if (mEntityManager->GetSignature(entity).test(WALL))
            return WALL;
        if (mEntityManager->GetSignature(entity).test(SHOOTER_MONSTER))
            return SHOOTER_MONSTER;
        if (mEntityManager->GetSignature(entity).test(FLYING_MONSTER))
            return FLYING_MONSTER;
        if (mEntityManager->GetSignature(entity).test(MONSTER))
            return MONSTER;
        if (mEntityManager->GetSignature(entity).test(P_BULLET))
            return P_BULLET;
        if (mEntityManager->GetSignature(entity).test(P_BULLET_CHARGED))
            return P_BULLET_CHARGED;
        if (mEntityManager->GetSignature(entity).test(M_BULLET))
            return M_BULLET;
        if (mEntityManager->GetSignature(entity).test(HUD))
            return HUD;
        if (mEntityManager->GetSignature(entity).test(BONUS_SPEED))
            return BONUS_SPEED;
        if (mEntityManager->GetSignature(entity).test(BONUS_SHOOT))
            return BONUS_SHOOT;
        return NONE;
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
