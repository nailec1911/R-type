/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeClient
*/

#include "RtypeClient.hpp"
#include <cstdint>
#include <vector>

#include "../../../gameEngine/Renderer/Elements.hpp"

void rtypeNetwork::RtypeClient::handleMessages(Renderer &renderer)
{
    while (getSizeReadQueue() > 0) {
        auto msg = popReadQueue();
        std::cout << msg << std::endl;
        if (msg.header.id == CustomMessageType::SNAPSHOT) {
            // updateGameData(gameServer::Snapshot<SnapshotData, 2>(msg.body), renderer);
        }
    }
};
void rtypeNetwork::RtypeClient::updateGameData(
    const gameServer::Snapshot<SnapshotData, 2> &newSnapshot,
    Renderer &renderer)
{
    for (auto item : newSnapshot.getElements()) {
        auto &spriteItem = renderer.getSpriteMap()[item.first];
        rndr::Vector2<float> vec = {
            static_cast<float>(item.second.getXY().x),
            static_cast<float>(item.second.getXY().y)};
        if (spriteItem) {
            renderer.setPosition(item.first, vec);
            return;
        }
        renderer.createSprite(
            item.first, eltInfo.at(item.second.getType()), vec);
    }
}
