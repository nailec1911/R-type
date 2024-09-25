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

#include "../../../ThreadSafeQueue.hpp"
#include "../AsioUdpClientInterface.hpp"
#include "rtypeRfc.hpp"

namespace rtypeNetwork {
class rtypeUdpClient
    : public network::AsioUdpClientInterface<rtypeNetwork::rtypeMessageType>
{
   public:
    rtypeUdpClient(const std::string &ip, std::uint16_t port);
    rtypeUdpClient(rtypeUdpClient &&) = delete;
    rtypeUdpClient(const rtypeUdpClient &) = delete;
    rtypeUdpClient &operator=(rtypeUdpClient &&) = delete;
    rtypeUdpClient &operator=(const rtypeUdpClient &) = delete;
    ~rtypeUdpClient() override;

    void sendMessage(
        const network::message<rtypeNetwork::rtypeMessageType> &msg) override;
    void readMessage() override {}

   private:
    void sendHeader() override;
    void sendBody() override;
    void readHeader() override {}
    void readBody() override {}

    asio::io_context m_ctx;
    asio::ip::udp::endpoint m_endpoint;
    asio::ip::udp::socket m_socket;
    std::unique_ptr<asio::io_context::work> m_work;
    std::thread m_thread;
    std::array<char, 1024> m_readBuffer;

    ThreadSafeQueue<network::message<rtypeNetwork::rtypeMessageType>>
        m_readQueue;
    ThreadSafeQueue<network::message<rtypeNetwork::rtypeMessageType>>
        m_sendQueue;
};
}  // namespace rtypeNetwork
