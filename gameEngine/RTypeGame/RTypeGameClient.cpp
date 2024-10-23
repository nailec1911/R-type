/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGameClient
*/

#include "RTypeGameClient.hpp"

#include <vector>

#include "../ECS/Managers/Component/StructComponent.hpp"

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

void gameEngine::RTypeGameClient::updateSystems()
{
    DataUpdate data = {{}, {}, {}, m_nbPlayers};

    for (auto &system : m_systems.getSystems()) {
        system->Update(m_mediator, data);
    }
    m_nbPlayers = data.nbPlayers;
}
