/*
** EPITECH PROJECT, 2024
** rts
** File description:
** entityManager
*/

#pragma once
#include "../../using.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

class EntityManager
{
   public:
    EntityManager() : mNumberEntity(0)
    {
        for (Entity entity = 0; entity < 500; entity += 1) {
            mEntitiesAvailable.push_back(entity);
        }
    }

    EntityManager(const EntityManager &) = default;
    EntityManager(EntityManager &&) = delete;
    EntityManager &operator=(const EntityManager &) = default;
    EntityManager &operator=(EntityManager &&) = delete;
    ~EntityManager() = default;

    Entity CreateEntity()
    {
        if (this->mNumberEntity >= MAX_ENTITIES)
            return ENTITY_ERROR;
        Entity id = this->mEntitiesAvailable.front();
        this->mEntitiesAvailable.erase(this->mEntitiesAvailable.begin());
        this->mNumberEntity += 1;
        return id;
    }

    Entity CreateEntityById(Entity id)
    {
        auto it = std::find(mEntitiesAvailable.begin(), mEntitiesAvailable.end(), id);
        if (it == mEntitiesAvailable.end())
            return ENTITY_ERROR;
        mEntitiesAvailable.erase(it);
        mNumberEntity += 1;
        return id;
    }

    void DestroyEntity(Entity entity)
    {
        if (mSignatures.erase(entity) == 0) {
            std::cout << "erase failed" << std::endl;
            return;
        }
        this->mEntitiesAvailable.push_back(entity);
        this->mNumberEntity -= 1;
    }

    void SetSignature(Entity entity, Signature signature)
    {
        this->mSignatures[entity] = signature;
    }

    Signature GetSignature(Entity entity)
    {
        return this->mSignatures[entity];
    }

    std::unordered_map<Entity, Signature> GetEntitiesSignatures(void)
    {
        return this->mSignatures;
    }

   private:
    std::vector<Entity> mEntitiesAvailable;
    std::unordered_map<Entity, Signature> mSignatures;
    std::uint32_t mNumberEntity;
};
