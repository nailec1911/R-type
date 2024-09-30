/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGame
*/

#include "RTypeGame.hpp"

#include <chrono>
#include <cstdint>
#include <unordered_map>

#include "ECS/using.hpp"
#include "Renderer/Elements.hpp"
#include "Snapshot/SnapshotData.hpp"

std::unordered_map<uint32_t, SnapshotData> gameEngine::RTypeGame::createSnapshots() const
{
    std::unordered_map<uint32_t, SnapshotData> snapshots;
    std::unordered_map<Entity, Signature> entities =
        m_mediator->GetEntitiesSignatures();

    for (auto elem : entities) {
        if (m_mediator->GetEntityRole(elem.first) == elementTypes::NONE)
            continue;
        auto position = m_mediator->GetComponent<Position>(elem.first);
        auto transform = m_mediator->GetComponent<Transform>(elem.first);
        auto type = m_mediator->GetEntityRole(elem.first);
        snapshots[elem.first] = {type, position.x, position.y, transform.velX, transform.velY, 0};
    }
    return snapshots;
}

void gameEngine::RTypeGame::initSystemSignature(const SystemType &type)
{
    Signature signature;
    if (type == SystemType::MOTION) {
        signature.set(m_mediator->GetComponentType<Player>());
        signature.set(m_mediator->GetComponentType<Bullet>());
        signature.set(m_mediator->GetComponentType<Monster>());
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
}

void gameEngine::RTypeGame::initGameRules(void)
{
    m_mediator = std::make_shared<Mediator>();
    m_mediator->RegisterComponent<Player>();
    m_mediator->RegisterComponent<Bullet>();
    m_mediator->RegisterComponent<Wall>();
    m_mediator->RegisterComponent<Monster>();
    m_mediator->RegisterComponent<HUDComp>();
    m_mediator->RegisterComponent<Transform>();
    m_mediator->RegisterComponent<Position>();
    m_mediator->RegisterComponent<BoundingBox>();

    m_systems.initSystems(m_mediator);

    initSystemSignature(SystemType::MOTION);
    initSystemSignature(SystemType::INPUTS);
    initSystemSignature(SystemType::COLLISION);
}

void gameEngine::RTypeGame::initHUDEntities(void)
{
    Entity score = m_mediator->CreateEntity();
    m_mediator->AddComponent<HUDComp>(score, HUDComp{.body = "Score: "});
    m_mediator->AddComponent<Position>(score, Position{.x = 0, .y = 0});
}

void gameEngine::RTypeGame::manageTime(void)
{
    auto current = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(current);
    if (current_time != m_start_time) {
        m_second += 1;
        m_start_time = current_time;
    }
}
