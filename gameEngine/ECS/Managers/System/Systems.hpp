/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Systems
*/

#pragma once
#include "../../Mediator.hpp"
#include <iostream>
#include "../../../Renderer/Events.hpp"

class MotionSystem : public System
{
public:
    void Update(float dt, std::shared_ptr<Mediator> mediator)
    {
        for (Entity const &entity : this->mEntities)
        {
            auto &transform = mediator->GetComponent<Transform>(entity);
            auto &position = mediator->GetComponent<Position>(entity);
            position.x += transform.velX * dt;
            position.y += transform.velY * dt;
        }
    }
};

class InputsPlayer : public System
{
public:
    void Update(std::shared_ptr<Mediator> mediator, std::vector<Event> events)
    {
        for (Entity const &entity : this->mEntities)
        {
            auto &transform = mediator->GetComponent<Transform>(entity);
            auto &position = mediator->GetComponent<Position>(entity);
            if (events.size() == 0)
            {
                transform.velX = 0;
                transform.velY = 0;
                break;
            }
            for (auto event : events)
            {
                if (event.key == EventKey::KeyLeft)
                    transform.velX = -4;
                if (event.key == EventKey::KeyRight)
                    transform.velX = 4;
                if (event.key == EventKey::KeyUp)
                    transform.velY = -4;
                if (event.key == EventKey::KeyDown)
                    transform.velY = 4;
                if (event.key == EventKey::KeyB)
                {
                    Entity bullet = mediator->CreateEntity();
                    mediator->AddComponent(bullet, Bullet{});
                    mediator->AddComponent(bullet, Transform{.velX = 4, .velY = 0});
                    mediator->AddComponent(bullet, Position{.x = position.x, .y = position.y});
                    //Send create sprite to client
                }
            }
        }
    }
};

class CollisionSystem : public System
{
public:
    // void DestroyEntities(Mediator &mediator, std::vector<size_t> &indexesToRemove, size_t entityOne, size_t entityTwo)
    // {
    //     if (mediator.GetEntityRole(mEntities[entityOne]) == "Player" && mediator.GetEntityRole(mEntities[entityTwo]) != "Bullet")
    //     {
    //         indexesToRemove.emplace_back(entityOne);
    //     }
    // }
    void Update(std::shared_ptr<Mediator> mediator)
    {
        std::vector<size_t> indexesToRemove;

        for (size_t i = 0; i < this->mEntities.size(); i += 1)
        {
            auto &positionEntityOne = mediator->GetComponent<Position>(mEntities[i]);
            auto &boxOne = mediator->GetComponent<BoundingBox>(mEntities[i]);
            float rightOne = positionEntityOne.x + boxOne.width;
            float bottomOne = positionEntityOne.y + boxOne.height;

            for (size_t z = i + 1; z < this->mEntities.size(); z += 1)
            {
                auto &positionEntityTwo = mediator->GetComponent<Position>(mEntities[z]);
                auto &boxTwo = mediator->GetComponent<BoundingBox>(mEntities[z]);
                float rightTwo = positionEntityTwo.x + boxTwo.width;
                float bottomTwo = positionEntityTwo.y + boxTwo.height;

                if (rightOne > positionEntityTwo.x && positionEntityOne.x < rightTwo &&
                    bottomOne > positionEntityTwo.y && positionEntityOne.y < bottomTwo)
                {
                    // DestroyEntities(mediator, indexesToRemove, i, z);
                }
            }
        }
        // for (size_t index : indexesToRemove)
            // mediator.DestroyEntity(mEntities[index]);
    }
};
