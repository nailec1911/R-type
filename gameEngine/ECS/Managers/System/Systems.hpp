/*
** EPITECH PROJECT, 2024
** rts
** File description:
** Systems
*/

#pragma once

#include <algorithm>
#include <any>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "../../../Renderer/Events.hpp"
#include "../../../updateDataStruct.hpp"
#include "../../Mediator.hpp"
#include "../Component/StructComponent.hpp"
#include "System.hpp"

inline std::shared_ptr<Mediator> castAnyTypeMediator(std::any med)
{
    try {
        auto mediator = std::any_cast<std::shared_ptr<Mediator>>(med);
        return mediator;
    } catch (const std::bad_any_cast &e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
}

class MotionSystem : public System
{
   public:
    void Update(std::any med, DataUpdate & /*data*/) override
    {
        // if (data.nbPlayers <= 0)
        //     return;
        auto mediator = castAnyTypeMediator(med);
        auto start = chrono::now();
        auto deltaDiff = start - m_lastUpdate;
        for (Entity const &entity : this->m_Entities) {
            auto &transform = mediator->GetComponent<Transform>(entity);
            auto &position = mediator->GetComponent<Position>(entity);
            position.x += transform.velX *
                          std::chrono::duration<float>(deltaDiff).count();
            position.y += transform.velY *
                          std::chrono::duration<float>(deltaDiff).count();
        }
        m_lastUpdate = start;
    }

   private:
    std::chrono::steady_clock::time_point m_lastUpdate;
};

class PlayerBorderSystem : public System
{
   public:
    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        for (auto &entity : this->m_Entities) {
            auto &position = mediator->GetComponent<Position>(entity);
            auto &box = mediator->GetComponent<BoundingBox>(entity);
            auto &transform = mediator->GetComponent<Transform>(entity);
            auto &chrono = mediator->GetComponent<Chrono>(entity);
            if (position.x > 1920 - box.width) {
                position.x = 1920 - box.width;
                transform.velX = 0;
                chrono.update(data.tick);
            }
            if (position.x < 0) {
                position.x = 0;
                transform.velX = 0;
                chrono.update(data.tick);
            }
            if (position.y < 0) {
                position.y = 0;
                transform.velY = 0;
                chrono.update(data.tick);
            }
            if (position.y > 1080 - box.height) {
                position.y = 1080 - box.height;
                transform.velY = 0;
                chrono.update(data.tick);
            }
        }
    }
};

class FlyingMonsterSystem : public MonstersSystem
{
   public:
    static bool changeDirection(
        const std::pair<Entity, std::chrono::steady_clock::time_point> &monster)
    {
        auto testShot = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            testShot - monster.second);
        return duration.count() > 600;
    }

    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        addMonsters();
        eraseMonsters();
        for (auto &monster : m_monster) {
            if (changeDirection(monster)) {
                auto &transform =
                    mediator->GetComponent<Transform>(monster.first);
                auto &chrono = mediator->GetComponent<Chrono>(monster.first);
                transform.velY *= -1;
                chrono.update(data.tick);
                monster.second = std::chrono::steady_clock::now();
            }
        }
    }
};

class ShootingMonsterSystem : public MonstersSystem
{
   public:
    static bool canShoot(
        const std::pair<Entity, std::chrono::steady_clock::time_point> &monster)
    {
        auto testShot = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            testShot - monster.second);
        return duration.count() > 600;
    }

    static void createMBullet(
        std::shared_ptr<Mediator> &mediator, const Entity &monster,
        uint32_t tick)
    {
        auto &position = mediator->GetComponent<Position>(monster);
        auto &boundingBox = mediator->GetComponent<BoundingBox>(monster);
        Entity bullet = mediator->CreateEntity();
        if (bullet == ENTITY_ERROR)
            return;
        mediator->AddComponent(bullet, BulletMonster{});
        mediator->AddComponent(bullet, Chrono{tick});
        mediator->AddComponent(bullet, Transform{.velX = -800, .velY = 0});
        mediator->AddComponent(bullet, BoundingBox{10, 10});
        mediator->AddComponent(
            bullet, Position{
                        .x = position.x,
                        .y = position.y + boundingBox.height / 2,
                        .initX = position.x,
                        .initY = position.y + boundingBox.height / 2});
    }

    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        addMonsters();
        eraseMonsters();
        for (auto &monster : m_monster) {
            if (canShoot(monster)) {
                createMBullet(mediator, monster.first, data.tick);
                monster.second = std::chrono::steady_clock::now();
            }
        }
    }
};

class InputsPlayer : public System
{
   public:
    void createPlayer(
        std::shared_ptr<Mediator> &mediator, const clientEvent &cEvent,
        DataUpdate &data)
    {
        Entity newPlayer = mediator->CreateEntity();
        if (newPlayer == ENTITY_ERROR)
            return;
        mediator->AddComponent<Player>(newPlayer, Player{.id = cEvent.id});
        mediator->AddComponent<Transform>(
            newPlayer, Transform{.velX = 0, .velY = 0});
        mediator->AddComponent<Position>(
            newPlayer,
            Position{.x = 500, .y = 540, .initX = 500, .initY = 540});
        mediator->AddComponent<BoundingBox>(newPlayer, BoundingBox{93, 33});
        m_players[cEvent.id] = {newPlayer, {}};
        data.clientsIdByEntities[cEvent.id] = newPlayer;
    }

    void resetPlayerVelocity(
        std::shared_ptr<Mediator> &mediator, DataUpdate &data)
    {
        for (auto &player : m_players) {
            auto &tranformComp =
                mediator->GetComponent<Transform>(player.second.first);
            auto &chrono = mediator->GetComponent<Chrono>(player.second.first);
            tranformComp.velX = 0;
            tranformComp.velY = 0;
            chrono.update(data.tick);
        }
    }

    static void createBullet(
        std::shared_ptr<Mediator> &mediator, const Entity &entity,
        uint32_t tick)
    {
        auto &position = mediator->GetComponent<Position>(entity);
        auto &boundingBox = mediator->GetComponent<BoundingBox>(entity);
        Entity bullet = mediator->CreateEntity();
        if (bullet == ENTITY_ERROR)
            return;
        mediator->AddComponent(bullet, Chrono{tick});
        mediator->AddComponent(bullet, BulletPlayer{});
        mediator->AddComponent(bullet, BoundingBox{10, 10});
        mediator->AddComponent(bullet, Transform{.velX = 800, .velY = 0});
        mediator->AddComponent(
            bullet, Position{
                        .x = position.x,
                        .y = position.y + boundingBox.height / 2,
                        .initX = position.x,
                        .initY = position.y + boundingBox.height / 2});
    }

    bool canShoot(const clientEvent &cEvent)
    {
        auto testShot = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            testShot - m_players[cEvent.id].second);
        return duration.count() > 300;
    }

    void eraseDeadPlayers()
    {
        for (auto it = m_players.begin(); it != m_players.end();) {
            Entity playerEntity = it->second.first;
            if (std::find(m_Entities.begin(), m_Entities.end(), playerEntity) ==
                m_Entities.end())
                it = m_players.erase(it);
            else
                ++it;
        }
    }

    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        eraseDeadPlayers();
        resetPlayerVelocity(mediator, data);
        while (!data.clientsEvents.empty()) {
            auto cEvent = data.clientsEvents.front();
            data.clientsEvents.pop();
            if (std::find_if(
                    data.deadPlayers.begin(), data.deadPlayers.end(),
                    [cEvent](uint32_t value) { return cEvent.id == value; }) !=
                data.deadPlayers.end())
                continue;
            if (m_players.find(cEvent.id) == m_players.end())
                createPlayer(mediator, cEvent, data);
            auto &transform =
                mediator->GetComponent<Transform>(m_players[cEvent.id].first);
            auto &chrono =
                mediator->GetComponent<Chrono>(m_players[cEvent.id].first);

            if (cEvent.event.key == EventKey::KeyLeft) {
                transform.velX = -600;
                chrono.update(data.tick);
            }
            if (cEvent.event.key == EventKey::KeyRight) {
                transform.velX = 600;
                chrono.update(data.tick);
            }
            if (cEvent.event.key == EventKey::KeyUp) {
                transform.velY = -600;
                chrono.update(data.tick);
            }
            if (cEvent.event.key == EventKey::KeyDown) {
                transform.velY = 600;
                chrono.update(data.tick);
            }
            if (cEvent.event.key == EventKey::KeyB && canShoot(cEvent)) {
                createBullet(mediator, m_players[cEvent.id].first, data.tick);
                m_players[cEvent.id].second = std::chrono::steady_clock::now();
            }
        }
        data.nbPlayers = m_players.size();
    }

   private:
    std::unordered_map<
        size_t, std::pair<Entity, std::chrono::steady_clock::time_point>>
        m_players;
};

class InputsPlayerClient : public System
{
   public:
    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        if (data.playerEntityId == -1)
            return;
        for (auto &entity : m_Entities) {
            if (entity != static_cast<Entity>(data.playerEntityId))
                continue;
            auto &transform = mediator->GetComponent<Transform>(entity);
            transform.velX = 0;
            transform.velY = 0;
            for (auto &event : data.events) {
                if (event.key == EventKey::KeyLeft)
                    transform.velX = -600;
                if (event.key == EventKey::KeyRight)
                    transform.velX = 600;
                if (event.key == EventKey::KeyUp)
                    transform.velY = -600;
                if (event.key == EventKey::KeyDown)
                    transform.velY = 600;
            }
        }
    }
};

class DestroyBullets : public System
{
   public:
    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        for (auto &entity : m_Entities) {
            auto &position = mediator->GetComponent<Position>(entity);
            if (position.x > 1940 || position.y > 1100)
                data.entitiesToRemove.push_back(entity);
        }
    }
};

class DestroyEntities : public System
{
   public:
    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        for (auto &entity : m_Entities) {
            auto &position = mediator->GetComponent<Position>(entity);
            auto &boundingBox = mediator->GetComponent<BoundingBox>(entity);
            if (position.x < 0 - boundingBox.width)
                data.entitiesToRemove.push_back(entity);
        }
    }
};

class CollisionSystem : public System
{
   public:
    void Update(std::any med, DataUpdate &data) override
    {
        auto mediator = castAnyTypeMediator(med);
        for (size_t i = 0; i < this->m_Entities.size(); i += 1) {
            auto &posA = mediator->GetComponent<Position>(m_Entities[i]);
            auto &boxA = mediator->GetComponent<BoundingBox>(m_Entities[i]);

            for (size_t z = i + 1; z < this->m_Entities.size(); z += 1) {
                auto &posB = mediator->GetComponent<Position>(m_Entities[z]);
                auto &boxB = mediator->GetComponent<BoundingBox>(m_Entities[z]);
                if (collide(posA, boxA, posB, boxB)) {
                    handleCollide(
                        mediator, data.entitiesToRemove, m_Entities[i],
                        m_Entities[z]);
                }
            }
        }
    }

   private:
    static bool isMonster(EntityName role)
    {
        return role == MONSTER || role == SHOOTER_MONSTER ||
               role == FLYING_MONSTER;
    }
    static void handleCollide(
        std::shared_ptr<Mediator> &mediator,
        std::vector<Entity> &entitiesToRemove, Entity &entityA, Entity &entityB)
    {
        EntityName roleA =
            static_cast<EntityName>(mediator->GetEntityRole(entityA));
        EntityName roleB =
            static_cast<EntityName>(mediator->GetEntityRole(entityB));

        if (roleA == roleB)
            return;
        if (roleA == WALL)
            return roleAWall(entitiesToRemove, entityA, entityB, roleB);
        if (roleA == PLAYER)
            return roleAPlayer(entitiesToRemove, entityA, entityB, roleB);
        if (isMonster(roleA))
            return roleAMonster(entitiesToRemove, entityA, entityB, roleB);
        if (roleA == P_BULLET)
            return roleABullet_player(
                entitiesToRemove, entityA, entityB, roleB);
        if (roleA == M_BULLET)
            return roleABullet_monster(
                entitiesToRemove, entityA, entityB, roleB);
    }

    static void roleAPlayer(
        std::vector<Entity> &entitiesToRemove, Entity &entityA, Entity &entityB,
        EntityName roleB)
    {
        if (roleB == P_BULLET)
            return;
        if (roleB == WALL || isMonster(roleB)) {
            entitiesToRemove.push_back(entityA);
            return;
        }
        if (roleB == M_BULLET) {
            entitiesToRemove.push_back(entityA);
            entitiesToRemove.push_back(entityB);
            return;
        }
    }

    static void roleABullet_player(
        std::vector<Entity> &entitiesToRemove, Entity &entityA, Entity &entityB,
        EntityName roleB)
    {
        if (roleB == PLAYER)
            return;
        if (roleB == WALL) {
            entitiesToRemove.push_back(entityA);
            return;
        }
        if (roleB == M_BULLET || isMonster(roleB)) {
            entitiesToRemove.push_back(entityA);
            entitiesToRemove.push_back(entityB);
            return;
        }
    }

    static void roleABullet_monster(
        std::vector<Entity> &entitiesToRemove, Entity &entityA, Entity &entityB,
        EntityName roleB)
    {
        if (isMonster(roleB))
            return;
        if (roleB == WALL) {
            entitiesToRemove.push_back(entityA);
            return;
        }
        if (roleB == P_BULLET || roleB == PLAYER) {
            entitiesToRemove.push_back(entityA);
            entitiesToRemove.push_back(entityB);
            return;
        }
    }

    static void roleAMonster(
        std::vector<Entity> &entitiesToRemove, Entity &entityA, Entity &entityB,
        EntityName roleB)
    {
        if (roleB == M_BULLET || roleB == WALL)
            return;
        if (roleB == PLAYER) {
            entitiesToRemove.push_back(entityB);
            return;
        }
        if (roleB == P_BULLET) {
            entitiesToRemove.push_back(entityA);
            entitiesToRemove.push_back(entityB);
            return;
        }
    }

    static void roleAWall(
        std::vector<Entity> &entitiesToRemove, Entity & /*entityA*/,
        Entity &entityB, EntityName roleB)
    {
        if (isMonster(roleB))
            return;
        entitiesToRemove.push_back(entityB);
    }

    static bool collide(
        Position &posA, BoundingBox &boxA, Position &posB, BoundingBox &boxB)
    {
        if (posA.x + boxA.width <= posB.x || posB.x + boxB.width <= posA.x) {
            return false;
        }
        if (posA.y + boxA.height <= posB.y || posB.y + boxB.height <= posA.y) {
            return false;
        }
        return true;
    }
};
