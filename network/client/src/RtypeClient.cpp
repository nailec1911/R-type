/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeClient
*/

#include "RtypeClient.hpp"

#include <cstdint>

#include "../../../gameEngine/Renderer/Sprites.hpp"
#include "../../../gameEngine/Renderer/IRenderer.hpp"

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
        if (renderer.getSpriteMap().find(item.first) ==
            renderer.getSpriteMap().end()) {
                std::cout << item.second.getType() << std::endl;
            renderer.createSprite(
                item.first, spriteTypeToStr.at(item.second.getType()), vec);
            continue;
        }
        renderer.setPosition(item.first, vec);
    }
    return newSnapshot.getId();
}
