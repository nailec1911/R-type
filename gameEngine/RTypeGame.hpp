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

   private:
    std::shared_ptr<MotionSystem> motion;
    std::shared_ptr<InputsPlayer> inputsSystem;
    std::shared_ptr<CollisionSystem> collision;
    std::shared_ptr<DestroyBullets> destroyBullets;
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
        BULLETDESTRUCTION
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

    std::unordered_map<uint32_t, SnapshotData> updateSystems(
        std::queue<clientEvent> &clientsEvents);

   protected:
   private:
    int m_second{};
    time_t m_start_time{};
    std::shared_ptr<Mediator> m_mediator;
    gameEngine::SystemsFactory m_systems;
    std::thread m_timeManagerThread;

    void manageTime(void);
};
}  // namespace gameEngine
