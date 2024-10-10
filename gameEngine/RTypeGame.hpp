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
#include <vector>

#include "../network/server/src/LevelConfigParser.hpp"
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
        destroyEntities = mediator->RegisterSystem<DestroyEntities>();
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

    std::shared_ptr<DestroyEntities> getDestroyEntitiesSystem(void)
    {
        return this->destroyEntities;
    }

   private:
    std::shared_ptr<MotionSystem> motion;
    std::shared_ptr<InputsPlayer> inputsSystem;
    std::shared_ptr<CollisionSystem> collision;
    std::shared_ptr<DestroyBullets> destroyBullets;
    std::shared_ptr<ShootingMonsterSystem> shootingMonster;
    std::shared_ptr<FlyingMonsterSystem> flyingMonsterSystem;
    std::shared_ptr<PlayerBorderSystem> playerBorderSystem;
    std::shared_ptr<DestroyEntities> destroyEntities;
};

class RTypeGame
{
   public:
    RTypeGame() = default;
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
        DESTROYENTITIES
    };

    void initGameRules(void);
    void startGame();
    void initSystemSignature(const SystemType &type);
    void initHUDEntities(void);
    void createFromConfig(
        std::unordered_map<float, std::vector<entitySpawn>> &level)
    {
        if (level.find(m_second) == level.end())
            return;
        for (auto &elem : level.at(m_second))
            createEntity(elem.type, elem.vecPos);
        level.erase(m_second);
    }
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

    bool isGameStarted() const
    {
        return m_gameHasStarted;
    }

    size_t getNbPlayers() const
    {
        return m_nbPlayers;
    }

    std::vector<uint32_t> getDeadPlayers() const
    {
        return m_deadPlayers;
    }

    bool isGameDone() const
    {
        return m_gameIsDone;
    }

    void createEntity(const EntityName &name, std::pair<float, float> pos);

    void createWall(std::pair<float, float> &pos);
    void createShooterMonster(std::pair<float, float> &pos);
    void createFlyingMonster(std::pair<float, float> &pos);

    std::unordered_map<uint32_t, SnapshotData> updateSystems(
        std::queue<clientEvent> &clientsEvents,
        std::vector<uint32_t> &playersToRemove);

    void gameTrigger(float winCondition);

   protected:
   private:
    float m_second{};
    time_t m_start_time{};
    std::shared_ptr<Mediator> m_mediator;
    gameEngine::SystemsFactory m_systems;
    std::thread m_timeManagerThread;
    size_t m_nbPlayers{};
    std::vector<uint32_t> m_deadPlayers;
    bool m_gameHasStarted{};
    bool m_gameIsDone{};
    bool m_stopTimer{};

    void manageTime(void);
};
}  // namespace gameEngine
