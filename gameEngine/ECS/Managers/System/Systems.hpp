/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Systems
*/

#pragma once

#include <queue>
#include <unordered_map>

#include "../../../../network/server/src/RtypeServer.hpp"
#include "../../Mediator.hpp"
#include "../Component/StructComponent.hpp"

class MotionSystem : public System
{
   public:
    void Update(float dt, const std::shared_ptr<Mediator> &mediator)
    {
        for (Entity const &entity : this->m_Entities) {
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
    void Update(
        const std::shared_ptr<Mediator> &mediator,
        std::queue<rtypeNetwork::clientEvent> &clientsEvents)
    {
        for (auto &player : m_players) {
            auto &tranformComp =
                mediator->GetComponent<Transform>(player.second);
            tranformComp.velX = 0;
            tranformComp.velY = 0;
        }
        while (!clientsEvents.empty()) {
            auto cEvent = clientsEvents.front();
            clientsEvents.pop();
            if (m_players.find(cEvent.id) == m_players.end()) {
                Entity newPlayer = mediator->CreateEntity();
                mediator->AddComponent<Player>(
                    newPlayer, Player{.id = cEvent.id});
                mediator->AddComponent<Transform>(
                    newPlayer, Transform{.velX = 0, .velY = 0});
                mediator->AddComponent<Position>(
                    newPlayer, Position{.x = 500, .y = 540});
                m_players[cEvent.id] = newPlayer;
            }
            auto &transform =
                mediator->GetComponent<Transform>(m_players[cEvent.id]);
            auto &position =
                mediator->GetComponent<Position>(m_players[cEvent.id]);
            if (cEvent.event.key == EventKey::KeyLeft)
                transform.velX = -4;
            if (cEvent.event.key == EventKey::KeyRight)
                transform.velX = 4;
            if (cEvent.event.key == EventKey::KeyUp)
                transform.velY = -4;
            if (cEvent.event.key == EventKey::KeyDown)
                transform.velY = 4;
            if (cEvent.event.key == EventKey::KeyB) {
                Entity bullet = mediator->CreateEntity();
                mediator->AddComponent(bullet, Bullet{});
                mediator->AddComponent(bullet, Transform{.velX = 4, .velY = 0});
                mediator->AddComponent(
                    bullet, Position{.x = position.x, .y = position.y});
            }
        }
    }

   private:
    std::unordered_map<size_t, Entity> m_players;
};

class CollisionSystem : public System
{
   public:
    void Update(const std::shared_ptr<Mediator> &mediator)
    {
        std::vector<size_t> indexesToRemove;

        for (size_t i = 0; i < this->m_Entities.size(); i += 1) {
            auto &positionEntityOne =
                mediator->GetComponent<Position>(m_Entities[i]);
            auto &boxOne = mediator->GetComponent<BoundingBox>(m_Entities[i]);
            float rightOne = positionEntityOne.x + boxOne.width;
            float bottomOne = positionEntityOne.y + boxOne.height;

            for (size_t z = i + 1; z < this->m_Entities.size(); z += 1) {
                auto &positionEntityTwo =
                    mediator->GetComponent<Position>(m_Entities[z]);
                auto &boxTwo =
                    mediator->GetComponent<BoundingBox>(m_Entities[z]);
                float rightTwo = positionEntityTwo.x + boxTwo.width;
                float bottomTwo = positionEntityTwo.y + boxTwo.height;

                if (rightOne > positionEntityTwo.x &&
                    positionEntityOne.x < rightTwo &&
                    bottomOne > positionEntityTwo.y &&
                    positionEntityOne.y < bottomTwo) {
                    // DestroyEntities(mediator, indexesToRemove, i, z);
                }
            }
        }
    }
};
