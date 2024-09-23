/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#include "AsioUdpServer.hpp"

#include <asio/io_context.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <system_error>

network::AsioUdpServer::AsioUdpServer(std::uint32_t port)
    : m_socket(m_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), m_readBuffer()
{
    if (m_socket.is_open())
    {
        std::cout << "[+]Server started\n";
        m_work = std::make_unique<asio::io_context::work>(m_ctx);
        receiveClientsMessages();
        m_thread = std::thread([&]() {
                m_ctx.run();
        });
    }
}

network::AsioUdpServer::~AsioUdpServer()
{
    m_ctx.stop();
    if (m_thread.joinable()) m_thread.join();
    std::cout << "[+]Server closed." << std::endl;
}

void network::AsioUdpServer::receiveClientsMessages() {
    m_socket.async_receive_from(
        asio::buffer(m_readBuffer), m_remoteEndpoint,
        [this](std::error_code ec, std::size_t bytesRead) {
            if (!ec) {
                std::cout << "Bytes read: " << bytesRead << " Message: " << m_readBuffer.data() << std::endl;
                network::message<network::messageType> msg;
                msg.header.id = static_cast<network::messageType>(m_readBuffer[0]);
            } else {
                std::cerr << "[-]Error while receiving the message: " << ec.message() << std::endl;
            }
            receiveClientsMessages();
        });
}
