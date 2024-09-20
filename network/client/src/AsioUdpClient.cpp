/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpClient
*/

#include "AsioUdpClient.hpp"
#include <asio/ip/address.hpp>
#include <asio/registered_buffer.hpp>
#include <cstdint>
#include <iostream>

network::AsioUdpClient::AsioUdpClient(const std::string &ip, std::uint16_t port)
    : m_endpoint(asio::ip::address::from_string(ip), port), m_socket(m_ctx, m_endpoint.protocol()), m_readBuffer()
{
    m_socket.connect(m_endpoint);
}

void network::AsioUdpClient::sendMessage(network::message<network::messageType> &msg)
{
    m_socket.send(asio::buffer(msg.body));
}
