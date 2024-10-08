/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGame
*/

#pragma once

#include <cstdint>
#include <queue>
#include <thread>
#include <unordered_map>

#include "ECS/Managers/System/Systems.hpp"
#include "ECS/Mediator.hpp"
#include "ECS/using.hpp"
#include "Snapshot/SnapshotData.hpp"

namespace gameEngine {
class SystemsFactory
{
   public:
    void initSystems(const std::shared_ptr<Mediator> &mediator)
    {
        motion = mediator->RegisterSystem<MotionSystem>();
        inputsSystem = mediator->RegisterSystem<InputsPlayer>();
        collision = mediator->RegisterSystem<CollisionSystem>();
        destroyBullets = mediator->RegisterSystem<DestroyBullets>();
        shootingMonster = mediator->RegisterSystem<ShootingMonsterSystem>();
        flyingMonsterSystem = mediator->RegisterSystem<FlyingMonsterSystem>();
        playerBorderSystem = mediator->RegisterSystem<PlayerBorderSystem>();
    }
    std::shared_ptr<MotionSystem> getMotionSystem(void)
    {
        return this->motion;
    }
    std::shared_ptr<InputsPlayer> getInputsSystem(void)
    {
        return this->inputsSystem;
    }
    std::shared_ptr<CollisionSystem> getCollisionSystem(void)
    {
        return this->collision;
    }

    std::shared_ptr<DestroyBullets> getDestroyBulletSystem(void)
    {
        return this->destroyBullets;
    }

    std::shared_ptr<ShootingMonsterSystem> getShootingMonsterSystem(void)
    {
        return this->shootingMonster;
    }

    std::shared_ptr<FlyingMonsterSystem> getFlyingMonsterSystem(void)
    {
        return this->flyingMonsterSystem;
    }

    std::shared_ptr<PlayerBorderSystem> getPlayerBorderSystem(void)
    {
        return this->playerBorderSystem;
    }

   private:
    std::shared_ptr<MotionSystem> motion;
    std::shared_ptr<InputsPlayer> inputsSystem;
    std::shared_ptr<CollisionSystem> collision;
    std::shared_ptr<DestroyBullets> destroyBullets;
    std::shared_ptr<ShootingMonsterSystem> shootingMonster;
    std::shared_ptr<FlyingMonsterSystem> flyingMonsterSystem;
    std::shared_ptr<PlayerBorderSystem> playerBorderSystem;
};

class RTypeGame
{
   public:
    RTypeGame();
    ~RTypeGame();
    RTypeGame(const RTypeGame &) = delete;
    RTypeGame(RTypeGame &&) = delete;
    RTypeGame &operator=(const RTypeGame &) = delete;
    RTypeGame &operator=(RTypeGame &&) = delete;

    enum class SystemType
    {
        MOTION,
        INPUTS,
        COLLISION,
        BULLETDESTRUCTION,
        SHOOTERMONSTER,
        FLYINGMONSTER,
        PLAYERBORDER,
    };

    void initGameRules(void);
    void initSystemSignature(const SystemType &type);
    void initHUDEntities(void);

    std::unordered_map<uint32_t, SnapshotData> createSnapshots(
        std::vector<Entity> &entitiesToRemove);

    std::shared_ptr<Mediator> getMediator(void)
    {
        return m_mediator;
    }

    SystemsFactory getSystems(void)
    {
        return m_systems;
    }

    void createEntity(const EntityName &name, std::pair<float, float> pos);

    void createWall(std::pair<float, float> &pos);
    void createShooterMonster(std::pair<float, float> &pos);
    void createFlyingMonster(std::pair<float, float> &pos);

    std::unordered_map<uint32_t, SnapshotData> updateSystems(
        std::queue<clientEvent> &clientsEvents);

   protected:
   private:
    float m_second{};
    time_t m_start_time{};
    std::shared_ptr<Mediator> m_mediator;
    gameEngine::SystemsFactory m_systems;
    std::thread m_timeManagerThread;

    void manageTime(void);
};
}  // namespace gameEngine
