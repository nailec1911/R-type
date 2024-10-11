/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeClient
*/

#include "RtypeClient.hpp"

#include <cstdint>

#include "../../../gameEngine/Renderer/IRenderer.hpp"
#include "../../../gameEngine/Renderer/Sprites.hpp"

void rtypeNetwork::RtypeClient::handleMessages(Renderer &renderer)
{
    while (getSizeReadQueue() > 0) {
        auto msg = popReadQueue();
        if (msg.header.id == CustomMessageType::SNAPSHOT) {
            uint32_t id = updateGameData(
                gameServer::Snapshot<SnapshotData, 2>(msg.body), renderer);
            asun::message<CustomMessageType> ok;
            ok.header.id = CustomMessageType::SNAP_OK;
            ok << id;
            sendMessage(ok);
        }
        if (msg.header.id == CustomMessageType::DEAD)
            m_isPlayerDead = true;
        if (msg.header.id == CustomMessageType::WIN && !m_isPlayerDead)
            m_hasPlayerWon = true;
    }
};

uint32_t rtypeNetwork::RtypeClient::updateGameData(
    const gameServer::Snapshot<SnapshotData, 2> &newSnapshot,
    Renderer &renderer)
{
    for (auto item : newSnapshot.getElements()) {
        if (item.second.getDestroy() == 1) {
            renderer.getSpriteMap().erase(item.first);
            continue;
        }
        rndr::Vector2<float> vec = {
            static_cast<float>(item.second.getXY().x),
            static_cast<float>(item.second.getXY().y)};
        auto type = item.second.getType();
        if (spriteTypeToStr.find(type) == spriteTypeToStr.end())
            continue;
        if (renderer.getSpriteMap().find(item.first) ==
            renderer.getSpriteMap().end()) {
            if (type == PLAYER1)
                type = choosePlayerSprite();
            renderer.createSprite(item.first, spriteTypeToStr.at(type), vec);
            continue;
        }
        renderer.setPosition(item.first, vec);
    }
    return newSnapshot.getId();
}

spritesTypes rtypeNetwork::RtypeClient::choosePlayerSprite()
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
