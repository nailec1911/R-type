/*
** EPITECH PROJECT, 2024
** src
** File description:
** rtypeUdpServer
*/

#include "rtypeUdpServer.hpp"

#include <asio/io_context.hpp>
#include <asio/read.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <system_error>

#include "rtypeRfc.hpp"

rtypeNetwork::rtypeUdpServer::rtypeUdpServer(std::uint32_t port)
    : m_socket(m_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      m_readBuffer()
{
    if (m_socket.is_open()) {
        std::cout << "[+]Server started\n";
        m_work = std::make_unique<asio::io_context::work>(m_ctx);
        m_thread = std::thread([&]() { readHeader(), m_ctx.run(); });
    }
}

rtypeNetwork::rtypeUdpServer::~rtypeUdpServer()
{
    m_ctx.stop();
    if (m_thread.joinable())
        m_thread.join();
    std::cout << "[+]Server closed." << std::endl;
}

void rtypeNetwork::rtypeUdpServer::readBody()
{
    m_socket.async_receive_from(
        asio::buffer(m_tempRead.body.data(), m_tempRead.body.size()), m_remoteEndpoint,
        [this](std::error_code ec, [[maybe_unused]] std::size_t bytesRead) {
            if (!ec) {
                std::cout << m_tempRead << std::endl;
                readHeader();
            } else {
                std::cerr << "[-]Error while reading body." << std::endl;
            }
        });
}

void rtypeNetwork::rtypeUdpServer::readHeader()
{
    m_socket.async_receive_from(
        asio::buffer(
            &m_tempRead.header,
            sizeof(network::messageHeader<rtypeNetwork::rtypeMessageType>)),
        m_remoteEndpoint, [this](std::error_code ec, [[maybe_unused]] std::size_t bytesRead) {
            if (!ec) {
                if (m_tempRead.header.size > 0) {
                    m_tempRead.body.resize(m_tempRead.header.size);
                    readBody();
                }
            } else {
                std::cerr << "[-]Error while reading body." << std::endl;
            }
        });
}

void rtypeNetwork::rtypeUdpServer::receiveClientsMessages()
{
    m_socket.async_receive_from(
        asio::buffer(m_readBuffer), m_remoteEndpoint,
        [this](std::error_code ec, std::size_t bytesRead) {
            if (!ec) {
                std::cout << "BYTES: " << bytesRead << " message: ["
                          << m_readBuffer.data() << "]" << std::endl;
            } else {
                std::cerr << "[-]Error while receiving the message: "
                          << ec.message() << std::endl;
            }
            receiveClientsMessages();
        });
}
