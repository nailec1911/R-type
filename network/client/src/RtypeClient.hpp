/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeClient
*/

#pragma once

#include <cstdint>
#include <vector>

#include "../../../gameEngine/RTypeGame/RTypeGameClient.hpp"
#include "../../../gameEngine/Renderer/Renderer.hpp"
#include "../../../gameEngine/Snapshot/SnapshotData.hpp"
#include "../../Errors.hpp"
#include "../../gameServ/Snapshot.hpp"
#include "AsioUdpClient.hpp"

namespace rtypeNetwork {
class RtypeClient : public asun::AsioUdpClient<CustomMessageType>
{
   public:
    RtypeClient(const std::string &ip, uint16_t port)
        : asun::AsioUdpClient<CustomMessageType>(ip, port), m_playerEntityId(-1)
    {
    }
    RtypeClient(RtypeClient &&) = delete;
    RtypeClient(const RtypeClient &) = delete;
    RtypeClient &operator=(RtypeClient &&) = delete;
    RtypeClient &operator=(const RtypeClient &) = delete;
    ~RtypeClient() override = default;

    void handleMessages(gameEngine::RTypeGameClient &rType, std::vector<Entity> &entitiesToRemove);

    bool isPlayerDead() const
    {
        return m_isPlayerDead;
    }

    bool hasPlayerWon() const
    {
        return m_hasPlayerWon;
    }

    void setPlayerEntityId(Entity id)
    {
        m_playerEntityId = id;
    }

    int getPlayerEntityId() const
    {
        return m_playerEntityId;
    }

   private:
    uint32_t updateGameData(
        const gameServer::Snapshot<SnapshotData, 1> &newSnapshot,
        gameEngine::RTypeGameClient &rType, std::vector<Entity> &entitiesToRemove);

    bool m_isPlayerDead{};
    bool m_hasPlayerWon{};
    int m_playerEntityId;
};
}  // namespace rtypeNetwork
