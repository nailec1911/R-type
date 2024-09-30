/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeServer
*/

#pragma once

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "../../../gameEngine/Renderer/Events.hpp"
#include "../../../gameEngine/Snapshot/SnapshotData.hpp"
#include "../../gameServ/GameServer.hpp"

namespace rtypeNetwork {
class RtypeServer
{
   public:
    RtypeServer(uint16_t port, uint8_t maxClient)
        : m_maxClient(maxClient), m_gameServ(port)
    {
    }
    RtypeServer(RtypeServer &&) = delete;
    RtypeServer(const RtypeServer &) = delete;
    RtypeServer &operator=(RtypeServer &&) = delete;
    RtypeServer &operator=(const RtypeServer &) = delete;
    ~RtypeServer() = default;

    void handleMessages();

    std::queue<clientEvent> getClientsEvents()
    {
        return this->m_clientsEvents;
    }

    void start()
    {
        m_gameServ.start();
    }
    void sendMaster(const std::unordered_map<uint32_t, SnapshotData>& eltsMap, int other1, int other2)
    {
        m_snapId++;
        asun::message<CustomMessageType> msg{};

        m_gameServ.sendMaster(CustomMessageType::SNAPSHOT, gameServer::Snapshot<SnapshotData, 2>(
            m_snapId, {other1, other2}, eltsMap));
    }

   private:
    uint32_t m_snapId{0};
    uint8_t m_maxClient;
    std::queue<clientEvent> m_clientsEvents;
    gameServer::GameServer<SnapshotData, 2, CustomMessageType> m_gameServ;
    std::vector<uint32_t> m_connectedClients;

    void handleClientLogin(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientMove(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientShoot(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    bool isClientConnected(uint32_t clientId)
    {
        return std::find(
                   m_connectedClients.begin(), m_connectedClients.end(),
                   clientId) != m_connectedClients.end();
    }
};
}  // namespace rtypeNetwork
