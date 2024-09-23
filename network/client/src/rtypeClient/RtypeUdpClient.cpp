/*
** EPITECH PROJECT, 2024
** src
** File description:
** rtypeUdpClient
*/

#include "RtypeUdpClient.hpp"

#include <asio/io_context.hpp>
#include <asio/ip/address.hpp>
#include <asio/read.hpp>
#include <asio/registered_buffer.hpp>
#include <asio/write.hpp>
#include <cstdint>

#include "rtypeRfc.hpp"

rtypeNetwork::rtypeUdpClient::rtypeUdpClient(const std::string &ip,
                                             std::uint16_t port)
    : m_endpoint(asio::ip::address::from_string(ip), port),
      m_socket(m_ctx, m_endpoint.protocol()),
      m_readBuffer() {
    m_socket.connect(m_endpoint);
    if (m_socket.is_open()) {
        m_work = std::make_unique<asio::io_context::work>(m_ctx);
        m_thread = std::thread([&]() { m_ctx.run(); });
    }
}

rtypeNetwork::rtypeUdpClient::~rtypeUdpClient() {
    m_ctx.stop();
    if (m_thread.joinable()) m_thread.join();
}

void rtypeNetwork::rtypeUdpClient::sendBody() {
    m_socket.async_send_to(
        asio::buffer(m_sendQueue.front().body, m_sendQueue.front().body.size()),
        m_endpoint,
        [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
            if (!ec) {
                m_sendQueue.pop();
                if (!m_sendQueue.isEmpty()) {
                    sendHeader();
                }
            }
        });
}

void rtypeNetwork::rtypeUdpClient::sendHeader() {
    m_socket.async_send_to(
        asio::buffer(&m_sendQueue.front().header,
                     sizeof(network::message<rtypeNetwork::rtypeMessageType>)),
        m_endpoint,
        [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
            if (!ec) {
                if (m_sendQueue.front().body.size() > 0) {
                    sendBody();
                } else {
                    m_sendQueue.pop();
                    if (!m_sendQueue.isEmpty()) {
                        sendHeader();
                    }
                }
            }
        });
}

void rtypeNetwork::rtypeUdpClient::sendMessage(
    const network::message<rtypeNetwork::rtypeMessageType> &msg) {
    asio::post(m_ctx, [this, msg]() {
        bool canSend = !m_sendQueue.isEmpty();
        m_sendQueue.push(msg);
        if (!canSend) {
            sendHeader();
        }
    });
}
