/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGame
*/

#include "RTypeGame.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ECS/Managers/Component/StructComponent.hpp"
#include "ECS/Managers/System/Systems.hpp"
#include "ECS/using.hpp"
#include "Renderer/Events.hpp"
#include "Renderer/Sprites.hpp"
#include "Snapshot/SnapshotData.hpp"

gameEngine::RTypeGame::~RTypeGame()
{
    if (m_timeManagerThread.joinable())
        m_timeManagerThread.join();
}

void gameEngine::RTypeGame::startGame()
{
    m_timeManagerThread = std::thread([this]() { this->manageTime(); });
}

std::unordered_map<uint32_t, SnapshotData>
gameEngine::RTypeGame::createSnapshots(std::vector<Entity> &entitiesToRemove)
{
    std::unordered_map<uint32_t, SnapshotData> snapshots{};
    std::unordered_map<Entity, Signature> entities =
        m_mediator->GetEntitiesSignatures();

    while (!entitiesToRemove.empty()) {
        auto entity = entitiesToRemove.front();
        entitiesToRemove.erase(entitiesToRemove.begin());
        snapshots[entity] = {spritesTypes::EMPTY, 0, 0, 1};
    }

    for (auto elem : entities) {
        if (m_mediator->GetEntitySprite(elem.first) == spritesTypes::EMPTY)
            continue;
        auto position = m_mediator->GetComponent<Position>(elem.first);
        auto type = m_mediator->GetEntitySprite(elem.first);
        snapshots[elem.first] = {
            type, static_cast<int>(position.x), static_cast<int>(position.y),
            0};
    }
    return snapshots;
}

void gameEngine::RTypeGame::initSystemSignature(const SystemType &type)
{
    Signature signature;
    if (type == SystemType::MOTION) {
        signature.set(m_mediator->GetComponentType<Player>());
        signature.set(m_mediator->GetComponentType<BulletPlayer>());
        signature.set(m_mediator->GetComponentType<BulletMonster>());
        signature.set(m_mediator->GetComponentType<BulletPlayer>());
        signature.set(m_mediator->GetComponentType<BulletMonster>());
        signature.set(m_mediator->GetComponentType<Monster>());
        signature.set(m_mediator->GetComponentType<Wall>());
        m_mediator->SetSystemSignature<MotionSystem>(signature);
        return;
    }
    if (type == SystemType::INPUTS) {
        signature.set(m_mediator->GetComponentType<Player>());
        m_mediator->SetSystemSignature<InputsPlayer>(signature);
        return;
    }

    if (type == SystemType::COLLISION) {
        signature.set(m_mediator->GetComponentType<BoundingBox>());
        m_mediator->SetSystemSignature<CollisionSystem>(signature);
        return;
    }
    if (type == SystemType::BULLETDESTRUCTION) {
        signature.set(m_mediator->GetComponentType<BulletPlayer>());
        m_mediator->SetSystemSignature<DestroyBullets>(signature);
        return;
    }
    if (type == SystemType::SHOOTERMONSTER) {
        signature.set(m_mediator->GetComponentType<ShooterMonster>());
        m_mediator->SetSystemSignature<ShootingMonsterSystem>(signature);
        return;
    }
    if (type == SystemType::FLYINGMONSTER) {
        signature.set(m_mediator->GetComponentType<FlyingMonster>());
        m_mediator->SetSystemSignature<FlyingMonsterSystem>(signature);
        return;
    }
    if (type == SystemType::PLAYERBORDER) {
        signature.set(m_mediator->GetComponentType<Player>());
        m_mediator->SetSystemSignature<PlayerBorderSystem>(signature);
        return;
    }
    if (type == SystemType::DESTROYENTITIES) {
        signature.set(m_mediator->GetComponentType<Wall>());
        signature.set(m_mediator->GetComponentType<BulletMonster>());
        signature.set(m_mediator->GetComponentType<Monster>());
        m_mediator->SetSystemSignature<DestroyEntities>(signature);
        return;
    }
}

void gameEngine::RTypeGame::initGameRules(void)
{
    m_mediator = std::make_shared<Mediator>();
    m_mediator->RegisterComponent<Player>(PLAYER);
    m_mediator->RegisterComponent<BulletPlayer>(P_BULLET);
    m_mediator->RegisterComponent<BulletMonster>(M_BULLET);
    m_mediator->RegisterComponent<Wall>(WALL);
    m_mediator->RegisterComponent<Monster>(MONSTER);
    m_mediator->RegisterComponent<ShooterMonster>(SHOOTER_MONSTER);
    m_mediator->RegisterComponent<FlyingMonster>(FLYING_MONSTER);
    m_mediator->RegisterComponent<HUDComp>(HUD);
    m_mediator->RegisterComponent<Transform>(TRANSFORM);
    m_mediator->RegisterComponent<Position>(POSITION);
    m_mediator->RegisterComponent<BoundingBox>(BOUNDING_BOX);

    m_systems.initSystems(m_mediator);

    initSystemSignature(SystemType::MOTION);
    initSystemSignature(SystemType::INPUTS);
    initSystemSignature(SystemType::COLLISION);
    initSystemSignature(SystemType::BULLETDESTRUCTION);
    initSystemSignature(SystemType::SHOOTERMONSTER);
    initSystemSignature(SystemType::FLYINGMONSTER);
    initSystemSignature(SystemType::PLAYERBORDER);
    initSystemSignature(SystemType::DESTROYENTITIES);
}

void gameEngine::RTypeGame::initHUDEntities(void)
{
    Entity score = m_mediator->CreateEntity();
    m_mediator->AddComponent<HUDComp>(score, HUDComp{.body = "Score: "});
    m_mediator->AddComponent<Position>(score, Position{.x = 0, .y = 0});
}

void gameEngine::RTypeGame::manageTime(void)
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

std::unordered_map<uint32_t, SnapshotData> gameEngine::RTypeGame::updateSystems(
    std::queue<clientEvent> &clientsEvents,
    std::vector<uint32_t> &playersToRemove)
{
    std::vector<Entity> entitiesToRemove{};

    m_nbPlayers = getSystems().getInputsSystem()->Update(
        getMediator(), clientsEvents, m_deadPlayers);
    getSystems().getCollisionSystem()->Update(getMediator(), entitiesToRemove);
    getSystems().getDestroyBulletSystem()->Update(
        getMediator(), entitiesToRemove);
    getSystems().getShootingMonsterSystem()->Update(getMediator());
    getSystems().getFlyingMonsterSystem()->Update(getMediator());
    if (m_nbPlayers > 0)
        getSystems().getMotionSystem()->Update(getMediator());
    getSystems().getDestroyEntitiesSystem()->Update(
        getMediator(), entitiesToRemove);
    getSystems().getPlayerBorderSystem()->Update(getMediator());
    for (auto &entity : entitiesToRemove) {
        if (m_mediator->GetEntityRole(entity) == PLAYER) {
            auto &player = m_mediator->GetComponent<Player>(entity);
            m_deadPlayers.push_back(player.id);
            playersToRemove.push_back(player.id);
        }
        m_mediator->DestroyEntity(entity);
    }
    auto snapshots = createSnapshots(entitiesToRemove);
    return snapshots;
}

void gameEngine::RTypeGame::gameTrigger(float winCondition)
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

void gameEngine::RTypeGame::createEntity(
    const EntityName &name, std::pair<float, float> pos)
{
    if (name == WALL) {
        createWall(pos);
        return;
    }
    if (name == SHOOTER_MONSTER) {
        createShooterMonster(pos);
        return;
    }
    if (name == FLYING_MONSTER) {
        createFlyingMonster(pos);
        return;
    }
}

void gameEngine::RTypeGame::createWall(std::pair<float, float> &pos)
{
    Entity wall = m_mediator->CreateEntity();
    m_mediator->AddComponent<Wall>(wall, {});
    m_mediator->AddComponent<Position>(wall, {pos.first, pos.second});
    m_mediator->AddComponent<Transform>(wall, {-0.5, 0});
    m_mediator->AddComponent<BoundingBox>(wall, {138, 138});
}

void gameEngine::RTypeGame::createShooterMonster(std::pair<float, float> &pos)
{
    Entity monster = m_mediator->CreateEntity();
    m_mediator->AddComponent<Monster>(monster, {});
    m_mediator->AddComponent<ShooterMonster>(monster, {});
    m_mediator->AddComponent<Position>(monster, {pos.first, pos.second});
    m_mediator->AddComponent<Transform>(monster, {0, 0});
    m_mediator->AddComponent<BoundingBox>(monster, {93, 93});
}

void gameEngine::RTypeGame::createFlyingMonster(std::pair<float, float> &pos)
{
    Entity monster = m_mediator->CreateEntity();
    m_mediator->AddComponent<Monster>(monster, {});
    m_mediator->AddComponent<FlyingMonster>(monster, {});
    m_mediator->AddComponent<Position>(monster, {pos.first, pos.second});
    m_mediator->AddComponent<Transform>(monster, {-1, 3});
    m_mediator->AddComponent<BoundingBox>(monster, {60, 69});
}
