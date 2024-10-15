/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGame
*/

#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../network/server/src/LevelConfigParser.hpp"
#include "ECS/Managers/System/System.hpp"
#include "ECS/Mediator.hpp"
#include "ECS/using.hpp"
#include "Snapshot/SnapshotData.hpp"

namespace gameEngine {
class SystemsFactory
{
   public:
    template <typename T>
    void addSystem(const std::shared_ptr<Mediator> &mediator)
    {
        std::shared_ptr<ISystem> system = mediator->RegisterSystem<T>();
        m_systems.push_back(system);
    }

    std::vector<std::shared_ptr<ISystem>> getSystems() const
    {
        return m_systems;
    }

   private:
    std::vector<std::shared_ptr<ISystem>> m_systems;
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
