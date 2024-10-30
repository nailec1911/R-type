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
#include "../ECS/Managers/System/Systems.hpp"
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
    std::vector<Entity> &entitiesToRemove, uint32_t /*tick*/)
{
    std::unordered_map<uint32_t, SnapshotData> snapshots{};
    std::unordered_map<Entity, Signature> entities =
        m_mediator->GetEntitiesSignatures();

    while (!entitiesToRemove.empty()) {
        auto entity = entitiesToRemove.front();
        entitiesToRemove.erase(entitiesToRemove.begin());
        snapshots[entity] = {DESTROY, 0, 0, 0, 0, 0};
    }

    for (auto elem : entities) {
        auto role = m_mediator->GetEntityRole(elem.first);
        if (role == EntityName::NONE)
            continue;
        auto position = m_mediator->GetComponent<Position>(elem.first);
        auto speed = m_mediator->GetComponent<Transform>(elem.first);
        auto &chrono = m_mediator->GetComponent<Chrono>(elem.first);

        snapshots[elem.first] = {
            role,
            static_cast<int16_t>(
                chrono.wasUpdated() || role == PLAYER || role == FLYING_MONSTER
                    ? position.x
                    : position.initX),
            static_cast<int16_t>(
                chrono.wasUpdated() || role == PLAYER || role == FLYING_MONSTER
                    ? position.y
                    : position.initY),
            speed.velX,
            speed.velY,
            chrono.getLastUpdate()};
        chrono.setNotUpdated();
    }
    return snapshots;
}

void gameEngine::RTypeGameServer::initHUDEntities(void)
{
    Entity score = m_mediator->CreateEntity();
    if (score == ENTITY_ERROR)
        return;
    m_mediator->AddComponent<HUDComp>(score, HUDComp{.body = "Score: "});
    m_mediator->AddComponent<Position>(
        score, Position{.x = 0, .y = 0, .initX = 0, .initY = 0});
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
    std::vector<uint32_t> &playersToRemove, uint32_t tick,
    std::unordered_map<uint32_t, Entity> &clientsIdByEntities)
{
    DataUpdate data = {
        clientsEvents, m_deadPlayers, {}, m_nbPlayers, tick, {}, {}, {}};

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
    auto snapshots = createSnapshots(data.entitiesToRemove, tick);
    clientsEvents = {};
    m_deadPlayers = data.deadPlayers;
    m_nbPlayers = data.nbPlayers;
    clientsIdByEntities = data.clientsIdByEntities;
    return snapshots;
}

void gameEngine::RTypeGameServer::gameTrigger(float winCondition)
{
    if (!m_gameHasStarted) {
        m_gameHasStarted = true;
        m_stopTimer = true;
        startGame();
    }
    if (winCondition <= m_second)
        m_gameIsDone = true;
    // if ((m_nbPlayers <= 0 && m_gameHasStarted))
    //     m_stopTimer = false;
}

void gameEngine::RTypeGameServer::initGameRules(void)
{
    m_mediator = std::make_shared<Mediator>();
    m_mediator->RegisterComponent<Player>(PLAYER);
    m_mediator->RegisterComponent<Chrono>(CHRONO);
    m_mediator->RegisterComponent<BulletPlayer>(P_BULLET);
    m_mediator->RegisterComponent<BigBulletPlayer>(P_BULLET_CHARGED);
    m_mediator->RegisterComponent<BulletMonster>(M_BULLET);
    m_mediator->RegisterComponent<Wall>(WALL);
    m_mediator->RegisterComponent<Monster>(MONSTER);
    m_mediator->RegisterComponent<ShooterMonster>(SHOOTER_MONSTER);
    m_mediator->RegisterComponent<FlyingMonster>(FLYING_MONSTER);
    m_mediator->RegisterComponent<HUDComp>(HUD);
    m_mediator->RegisterComponent<Transform>(TRANSFORM);
    m_mediator->RegisterComponent<Position>(POSITION);
    m_mediator->RegisterComponent<BoundingBox>(BOUNDING_BOX);

    m_systems.addSystem<InputsPlayer>(m_mediator);
    m_systems.addSystem<CollisionSystem>(m_mediator);
    m_systems.addSystem<DestroyBullets>(m_mediator);
    m_systems.addSystem<ShootingMonsterSystem>(m_mediator);
    m_systems.addSystem<FlyingMonsterSystem>(m_mediator);
    m_systems.addSystem<MotionSystem>(m_mediator);
    m_systems.addSystem<DestroyEntities>(m_mediator);
    m_systems.addSystem<PlayerBorderSystem>(m_mediator);

    initSystemSignature(SystemType::MOTION);
    initSystemSignature(SystemType::INPUTS);
    initSystemSignature(SystemType::COLLISION);
    initSystemSignature(SystemType::BULLETDESTRUCTION);
    initSystemSignature(SystemType::SHOOTERMONSTER);
    initSystemSignature(SystemType::FLYINGMONSTER);
    initSystemSignature(SystemType::PLAYERBORDER);
    initSystemSignature(SystemType::DESTROYENTITIES);
}
