/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeClient
*/

#include "RtypeClient.hpp"

#include <cstdint>
#include <format>
#include <vector>

#include "../../../gameEngine/ECS/Managers/Component/StructComponent.hpp"

void rtypeNetwork::RtypeClient::handleMessages(
    gameEngine::RTypeGameClient &rType, std::vector<Entity> &entitiesToRemove)
{
    while (getSizeReadQueue() > 0) {
        auto msg = popReadQueue();

        if (msg.header.id == CustomMessageType::SNAPSHOT) {
            try
            {
                uint32_t id = updateGameData(
                    gameServer::Snapshot<SnapshotData, 1>(msg.body), rType,
                    entitiesToRemove);
                asun::message<CustomMessageType> ok;
                ok.header.id = CustomMessageType::SNAP_OK;
                ok << id;
                sendMessage(ok);
            }
            catch (const std::format_error &e)
            {
                std::cerr << e.what() << std::endl;
                return;
            }
        }
        if (msg.header.id == CustomMessageType::DEAD)
            m_isPlayerDead = true;
        if (msg.header.id == CustomMessageType::WIN && !m_isPlayerDead)
            m_hasPlayerWon = true;
        if (msg.header.id == CustomMessageType::YOUR_PLAYER) {
            Entity playerId = 0;
            msg >> playerId;
            setPlayerEntityId(playerId);
        }
    }
};

uint32_t rtypeNetwork::RtypeClient::updateGameData(
    const gameServer::Snapshot<SnapshotData, 1> &newSnapshot,
    gameEngine::RTypeGameClient &rType, std::vector<Entity> &entitiesToRemove)
{
    auto entities = rType.getMediator()->GetEntitiesSignatures();

    uint32_t tick = newSnapshot.getOthers()[0];

    for (auto item : newSnapshot.getElements()) {
        if (item.second.getType() == DESTROY) {
            rType.getMediator()->DestroyEntity(item.first);
            entitiesToRemove.push_back(item.first);
            continue;
        }
        auto type = item.second.getType();
        auto pos = item.second.getXY();
        auto vel = item.second.getVelocity();

        pos.first +=
            static_cast<int>(vel.first * (tick - item.second.getTick())) *
            1 / 32;
        pos.second +=
            static_cast<int>(vel.second * (tick - item.second.getTick())) *
            1 / 32;
        if (entities.find(item.first) == entities.end()) {
            if (type == PLAYER)
                rType.getNbPlayers() += 1;
            rType.createEntity(
                type, {pos.first, pos.second}, static_cast<int>(item.first),
                item.second.getTick());
            continue;
        }
        auto &position =
            rType.getMediator()->GetComponent<Position>(item.first);
        position.x = static_cast<float>(pos.first);
        position.y = static_cast<float>(pos.second);
        auto &velocity =
            rType.getMediator()->GetComponent<Transform>(item.first);
        velocity.velX = vel.first;
        velocity.velY = vel.second;
    }
    return newSnapshot.getId();
}
