/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGameClient
*/

#include "RTypeGameClient.hpp"

#include <cstdint>
#include <vector>

#include "../ECS/Managers/Component/StructComponent.hpp"
#include "../ECS/Managers/System/Systems.hpp"

void gameEngine::RTypeGameClient::updateRendererState(
    Renderer &renderer, std::vector<Entity> &entitiesToRemove)
{
    auto entities = m_mediator->GetEntitiesSignatures();

    for (auto rmEntitiy : entitiesToRemove)
        renderer.getSpriteMap().erase(rmEntitiy);
    for (auto &entity : entities) {
        auto type = m_mediator->GetEntitySprite(entity.first);
        if (type == spritesTypes::EMPTY)
            continue;
        auto &pos = m_mediator->GetComponent<Position>(entity.first);
        rndr::Vector2<float> vec = {
            static_cast<float>(pos.x), static_cast<float>(pos.y)};
        if (renderer.getSpriteMap().find(entity.first) ==
            renderer.getSpriteMap().end()) {
            if (type == PLAYER1)
                type = choosePlayerSprite();
            renderer.createSprite(entity.first, spriteTypeToStr.at(type), vec);
            continue;
        }
        renderer.setPosition(entity.first, vec);
    }
}

spritesTypes gameEngine::RTypeGameClient::choosePlayerSprite()
{
    spritesTypes type = PLAYER1;
    if (m_nbPlayers == 1)
        type = PLAYER2;
    if (m_nbPlayers == 2)
        type = PLAYER3;
    if (m_nbPlayers == 3)
        type = PLAYER4;
    if (m_nbPlayers == 4)
        type = PLAYER5;
    m_nbPlayers += 1;
    return type;
}

void gameEngine::RTypeGameClient::updateSystems(
    uint32_t tick, std::vector<Event> &events, int playerEntityId)
{
    DataUpdate data = {{},   {}, {},     m_nbPlayers,
                       tick, {}, events, playerEntityId};

    for (auto &system : m_systems.getSystems())
        system->Update(m_mediator, data);
}

void gameEngine::RTypeGameClient::initGameRules(void)
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

    m_systems.addSystem<InputsPlayerClient>(m_mediator);
    m_systems.addSystem<MotionSystem>(m_mediator);
    m_systems.addSystem<PlayerBorderSystem>(m_mediator);

    initSystemSignature(SystemType::MOTION);
    initSystemSignature(SystemType::INPUTS_CLIENT);
    initSystemSignature(SystemType::PLAYERBORDER);
}
