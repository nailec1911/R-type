/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpClient
*/

#include "AsioUdpClient.hpp"
#include <asio/ip/address.hpp>
#include <asio/read.hpp>
#include <asio/registered_buffer.hpp>
#include <asio/write.hpp>
#include <cstdint>

network::AsioUdpClient::AsioUdpClient(const std::string &ip, std::uint16_t port)
    : m_endpoint(asio::ip::address::from_string(ip), port), m_socket(m_ctx, m_endpoint.protocol()), m_readBuffer()
{
    m_socket.connect(m_endpoint);
}

void network::AsioUdpClient::sendBody()
{
    asio::async_write(m_socket, asio::buffer(&m_sendQueue.front().header, sizeof(network::message<network::messageType>)),
    [this](std::error_code ec, [[maybe_unused]] std::size_t length)
    {
        if (!ec)
        {
            m_sendQueue.pop();
            if (!m_sendQueue.isEmpty())
            {
                sendHeader();
            }
        }
    });
}

void network::AsioUdpClient::sendHeader()
{
    asio::async_write(m_socket, asio::buffer(&m_sendQueue.front().header, sizeof(network::message<network::messageType>)),
    [this](std::error_code ec, [[maybe_unused]] std::size_t length)
    {
        if (!ec)
        {
            if (m_sendQueue.front().body.size() > 0)
            {
                sendBody();
            } else {
                m_sendQueue.pop();
                if (!m_sendQueue.isEmpty())
                {
                    sendHeader();
                }
            }
        }
    });
}

void network::AsioUdpClient::sendMessage(const network::message<network::messageType> &msg)
{
    asio::post(m_ctx, [this, msg]()
    {
        if (m_sendQueue.isEmpty())
        {
            m_sendQueue.push(msg);
            sendHeader();
        }
    });
}
