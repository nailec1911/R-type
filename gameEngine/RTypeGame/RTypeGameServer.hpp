/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGameServer
*/

#pragma once

#include <cstdint>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../../network/server/src/LevelConfigParser.hpp"
#include "ARTypeGame.hpp"
#include "../ECS/using.hpp"
#include "../Snapshot/SnapshotData.hpp"

namespace gameEngine {

class RTypeGameServer : public ARTypeGame
{
   public:
    RTypeGameServer() = default;
    ~RTypeGameServer();
    RTypeGameServer(const RTypeGameServer &) = delete;
    RTypeGameServer(RTypeGameServer &&) = delete;
    RTypeGameServer &operator=(const RTypeGameServer &) = delete;
    RTypeGameServer &operator=(RTypeGameServer &&) = delete;

    void startGame();
    void initHUDEntities(void);
    void createFromConfig(
        std::unordered_map<float, std::vector<entitySpawn>> &level)
    {
        if (level.find(m_second) == level.end())
            return;
        for (auto &elem : level.at(m_second))
            createEntity(elem.type, elem.vecPos, -1);
        level.erase(m_second);
    }
    std::unordered_map<uint32_t, SnapshotData> createSnapshots(
        std::vector<Entity> &entitiesToRemove);

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

    std::unordered_map<uint32_t, SnapshotData> updateSystems(
        std::queue<clientEvent> &clientsEvents,
        std::vector<uint32_t> &playersToRemove);

    void gameTrigger(float winCondition);

   protected:
   private:
    float m_second{};
    time_t m_start_time{};
    std::thread m_timeManagerThread;
    size_t m_nbPlayers{};
    std::vector<uint32_t> m_deadPlayers;
    bool m_gameHasStarted{};
    bool m_gameIsDone{};
    bool m_stopTimer{};

    void manageTime(void);
};
}  // namespace gameEngine
