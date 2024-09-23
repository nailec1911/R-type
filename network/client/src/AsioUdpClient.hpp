/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpClient
*/

#pragma once

#include <array>
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <cstdint>
#include <memory>

#include "../../ThreadSafeQueue.hpp"
#include "AsioUdpClientInterface.hpp"

namespace network {
class AsioUdpClient : public AsioUdpClientInterface<network::messageType> {
   public:
    AsioUdpClient(const std::string &ip, std::uint16_t port);
    AsioUdpClient(AsioUdpClient &&) = delete;
    AsioUdpClient(const AsioUdpClient &) = delete;
    AsioUdpClient &operator=(AsioUdpClient &&) = delete;
    AsioUdpClient &operator=(const AsioUdpClient &) = delete;
    ~AsioUdpClient() override = default;

    void sendMessage(const message<network::messageType> &msg) override;
    void readMessages() override {}

   private:
    asio::io_context m_ctx;
    asio::ip::udp::endpoint m_endpoint;
    asio::ip::udp::socket m_socket;
    std::unique_ptr<asio::io_context::work> m_work;
    std::array<char, 1024> m_readBuffer;

    void sendHeader();
    void sendBody();
    // TODO
    void readHeader() {}
    // TODO
    void readBody() {}

    ThreadSafeQueue<message<network::messageType>> m_readQueue;
    ThreadSafeQueue<message<network::messageType>> m_sendQueue;
};
}  // namespace network
