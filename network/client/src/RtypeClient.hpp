/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeClient
*/

#pragma once

#include <cstdint>

#include "../../../gameEngine/Renderer/Renderer.hpp"
#include "../../../gameEngine/Snapshot/SnapshotData.hpp"
#include "../../gameServ/Snapshot.hpp"
#include "AsioUdpClient.hpp"

namespace rtypeNetwork {
class RtypeClient : public asun::AsioUdpClient<CustomMessageType>
{
   public:
    RtypeClient(const std::string &ip, uint16_t port)
        : asun::AsioUdpClient<CustomMessageType>(ip, port)
    {
    }
    RtypeClient(RtypeClient &&) = delete;
    RtypeClient(const RtypeClient &) = delete;
    RtypeClient &operator=(RtypeClient &&) = delete;
    RtypeClient &operator=(const RtypeClient &) = delete;
    ~RtypeClient() override = default;

    void handleMessages(Renderer &renderer);

    bool isPlayerDead() const
    {
        return m_isPlayerDead;
    }

   private:
    static uint32_t updateGameData(
        const gameServer::Snapshot<SnapshotData, 2> &newSnapshot,
        Renderer &renderer);
    bool m_isPlayerDead{};
};
}  // namespace rtypeNetwork
