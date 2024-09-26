/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeServer
*/

#pragma once

#include <asio/ip/udp.hpp>
#include <cstdint>
#include <queue>
#include <unordered_map>

#include "AsioUdpServer.hpp"
#include "../../../gameEngine/Renderer/Events.hpp"

namespace rtypeNetwork {

enum class CustomMessageType
{
    SHOOT,
    MOVE,
    OK,
    KO,
    LOGIN,
};

struct clientEvent
{
    uint16_t id;
    Event event;
};

using clientSession = struct clientSession
{
    std::string clientProtocol;
    asio::ip::udp::endpoint endpoint;
    std::chrono::steady_clock::time_point lastHeartBeat;
};

class RtypeServer : public asun::AsioUdpServer<CustomMessageType>
{
   public:
    RtypeServer(uint16_t port)
        : asun::AsioUdpServer<CustomMessageType>(port)
    {
    }
    RtypeServer(RtypeServer &&) = delete;
    RtypeServer(const RtypeServer &) = delete;
    RtypeServer &operator=(RtypeServer &&) = delete;
    RtypeServer &operator=(const RtypeServer &) = delete;
    ~RtypeServer() override = default;

    void handleMessages() override;

    std::queue<clientEvent> getClientsEvents(void)
    {
        return this->m_clientsEvents;
    }

   private:
    uint32_t m_id{};
    std::unordered_map<uint32_t, clientSession> m_clients;
    std::queue<clientEvent> m_clientsEvents;

    void handleClientLogin(std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> &msg);
    void handleClientMove(std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> &msg);
    void handleClientShoot(std::pair<asio::ip::udp::endpoint, asun::message<CustomMessageType>> &msg);

};
}  // namespace rtypeNetwork
