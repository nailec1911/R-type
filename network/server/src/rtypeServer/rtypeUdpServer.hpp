/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#pragma once

#include <asio/executor_work_guard.hpp>
#include <asio/io_context.hpp>
#include <cstdint>
#include <queue>
#include <thread>
#include <unordered_map>

#include "../../../Message.hpp"
#include "../../../ThreadSafeQueue.hpp"
#include "../AsioUdpServerInterface.hpp"
#include "clientsEvents.hpp"
#include "rtypeRfc.hpp"

namespace rtypeNetwork {

constexpr std::size_t BUFFER_SIZE = 1024;

struct clientSession
{
    std::string clientAddressPort;
    asio::ip::udp::endpoint endpoint;
    std::chrono::steady_clock::time_point last_heartbeat;
};

class rtypeUdpServer
    : public network::AsioUdpServerInterface<rtypeNetwork::rtypeMessageType>
{
   public:
    rtypeUdpServer(std::uint32_t port);
    rtypeUdpServer(rtypeUdpServer &&) = delete;
    rtypeUdpServer(const rtypeUdpServer &) = delete;
    rtypeUdpServer &operator=(rtypeUdpServer &&) = delete;
    rtypeUdpServer &operator=(const rtypeUdpServer &) = delete;
    ~rtypeUdpServer() override;

    std::queue<clientEvent> getClientsEvents(void) const;

   private:
    asio::io_context m_ctx;
    asio::ip::udp::socket m_socket;

    std::unordered_map<uint16_t, struct clientSession> m_clients;

    std::array<char, BUFFER_SIZE> m_readBuffer;
    ThreadSafeQueue<network::message<rtypeNetwork::rtypeMessageType>>
        m_readQueue;
    std::thread m_thread;
    asio::ip::udp::endpoint m_remoteEndpoint;
    std::unique_ptr<asio::io_context::work> m_work;

    network::message<rtypeNetwork::rtypeMessageType> m_tempRead;
    uint16_t m_id;
    std::queue<clientEvent> m_clientsEvents;

    void handleClientLogin(const std::string &endpointData);
    void handleMoveClient(const std::string &endpointData);
    void handleShootClient(const std::string &endpointData);

    // TODO ->
    void readHeader();
    void readBody();
    void sendMessagesToAllClients(
        const network::message<rtypeNetwork::rtypeMessageType> &msg) override
    {
        (void)msg;
    }
    void sendMessagesToClient(
        const network::message<rtypeNetwork::rtypeMessageType> &msg,
        uint16_t clientId) override;
    void sendHeader() {}
    void sendBody() {}
};
}  // namespace rtypeNetwork
