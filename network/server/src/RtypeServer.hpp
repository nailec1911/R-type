/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeServer
*/

#pragma once

#include <unordered_map>
#include "AsioUdpServer.hpp"

namespace rtypeNetwork {

enum class CustomMessageType
{
    OK,
    KO,
};

using clientSession = struct clientSession {
    std::string username;
    asio::ip::udp::endpoint endpoint;
};

class RtypeServer : public asun::AsioUdpServer<CustomMessageType>
{
   public:
    RtypeServer(uint16_t port)
        : asun::AsioUdpServer<CustomMessageType>(port)
    {}
    RtypeServer(RtypeServer &&) = delete;
    RtypeServer(const RtypeServer &) = delete;
    RtypeServer &operator=(RtypeServer &&) = delete;
    RtypeServer &operator=(const RtypeServer &) = delete;
    ~RtypeServer() override = default;

    void handleMessages() override {
        
    }

   private:
    std::unordered_map<asio::ip::udp::endpoint, clientSession> m_clients;
};
}  // namespace rtypeNetwork
