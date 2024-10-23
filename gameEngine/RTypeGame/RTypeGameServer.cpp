/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGameServer
*/

#include "RTypeGameServer.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../ECS/Managers/Component/StructComponent.hpp"
#include "../ECS/using.hpp"
#include "../Renderer/Events.hpp"
#include "../Snapshot/SnapshotData.hpp"
#include "../updateDataStruct.hpp"

std::vector<std::string> getAllConfigFile(char const *filepath)
{
    std::vector<std::string> files;

    for (const auto &elem : std::filesystem::directory_iterator(filepath)) {
        files.push_back(elem.path().string());
    }
    return files;
}

gameEngine::RTypeGameServer::~RTypeGameServer()
{
    if (m_timeManagerThread.joinable())
        m_timeManagerThread.join();
}

void gameEngine::RTypeGameServer::startGame()
{
    m_timeManagerThread = std::thread([this]() { this->manageTime(); });
}

std::unordered_map<uint32_t, SnapshotData>
gameEngine::RTypeGameServer::createSnapshots(
    std::vector<Entity> &entitiesToRemove)
{
    std::unordered_map<uint32_t, SnapshotData> snapshots{};
    std::unordered_map<Entity, Signature> entities =
        m_mediator->GetEntitiesSignatures();

    while (!entitiesToRemove.empty()) {
        auto entity = entitiesToRemove.front();
        entitiesToRemove.erase(entitiesToRemove.begin());
        snapshots[entity] = {m_mediator->GetEntityRole(entity), 0, 0, 0, 0, 0, 1};
    }

    for (auto elem : entities) {
        auto role = m_mediator->GetEntityRole(elem.first);
        if (role == EntityName::NONE)
            continue;
        auto position = m_mediator->GetComponent<Position>(elem.first);
        snapshots[elem.first] = {
            role,
            static_cast<int16_t>(position.x),
            static_cast<int16_t>(position.y),
            0,
            0,
            0,
            0};
    }
    return snapshots;
}

void gameEngine::RTypeGameServer::initHUDEntities(void)
{
    Entity score = m_mediator->CreateEntity();
    m_mediator->AddComponent<HUDComp>(score, HUDComp{.body = "Score: "});
    m_mediator->AddComponent<Position>(score, Position{.x = 0, .y = 0});
}

void gameEngine::RTypeGameServer::manageTime(void)
{
    const std::chrono::seconds interval(1);
    auto start = std::chrono::high_resolution_clock::now();

    while (m_stopTimer) {
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(now - start)
                .count();
        m_second = static_cast<float>(elapsed);
        std::this_thread::sleep_for(interval);
    }
}

std::unordered_map<uint32_t, SnapshotData>
gameEngine::RTypeGameServer::updateSystems(
    std::queue<clientEvent> &clientsEvents,
    std::vector<uint32_t> &playersToRemove)
{
    DataUpdate data = {clientsEvents, m_deadPlayers, {}, m_nbPlayers};

    for (auto &system : m_systems.getSystems())
        system->Update(getMediator(), data);
    for (auto &entity : data.entitiesToRemove) {
        if (m_mediator->GetEntityRole(entity) == PLAYER) {
            auto &player = m_mediator->GetComponent<Player>(entity);
            data.deadPlayers.push_back(player.id);
            playersToRemove.push_back(player.id);
        }
        m_mediator->DestroyEntity(entity);
    }
    auto snapshots = createSnapshots(data.entitiesToRemove);
    clientsEvents = {};
    m_deadPlayers = data.deadPlayers;
    m_nbPlayers = data.nbPlayers;
    return snapshots;
}

void gameEngine::RTypeGameServer::gameTrigger(float winCondition)
{
    if (m_nbPlayers > 0 && !m_gameHasStarted) {
        m_gameHasStarted = true;
        m_stopTimer = true;
        startGame();
    }
    if (winCondition <= m_second)
        m_gameIsDone = true;
    if ((m_nbPlayers <= 0 && m_gameHasStarted))
        m_stopTimer = false;
}
