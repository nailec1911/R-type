/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#pragma once

#include <asio.hpp>
#include <asio/socket_base.hpp>
#include <cstdint>
#include <iostream>

#include "../../Message.hpp"
#include "../../ThreadSafeQueue.hpp"
#include "../../server/src/AsioUdpServer.hpp"

namespace asun {

template <typename T>
class AsioUdpClient : public AsioNetworkThread
{
   public:
    AsioUdpClient(const std::string &ip, uint16_t port)
        : AsioNetworkThread(port),
          m_serverEndpoint(asio::ip::address::from_string(ip), port),
          m_socket(m_ctx, m_serverEndpoint.protocol())
    {
        m_socket.connect(m_serverEndpoint);
        m_socket.set_option(asio::socket_base::receive_buffer_size(65536));
        readHeader();
    }
    AsioUdpClient(AsioUdpClient &&) = delete;
    AsioUdpClient(const AsioUdpClient &) = delete;
    AsioUdpClient &operator=(AsioUdpClient &&) = delete;
    AsioUdpClient &operator=(const AsioUdpClient &) = delete;
    ~AsioUdpClient() override
    {
        AsioNetworkThread::stop();
    }

    void sendMessage(const message<T> &msg)
    {
        asio::post(m_ctx, [this, msg]() {
            bool canSend = !m_sendQueue.isEmpty();
            m_sendQueue.push(msg);
            if (!canSend) {
                sendHeader();
            }
        });
    }

   protected:
    asun::message<T> popReadQueue(void)
    {
        return m_readQueue.pop();
    }

    size_t getSizeReadQueue(void) const
    {
        return m_readQueue.size();
    }

   private:
    void sendBody()
    {
        m_socket.async_send_to(
            asio::buffer(
                m_sendQueue.front().body, m_sendQueue.front().body.size()),
            m_serverEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    m_sendQueue.pop();
                    if (!m_sendQueue.isEmpty()) {
                        sendHeader();
                    }
                } else {
                    std::cerr << ec.message() << std::endl;
                }
            });
    }

    void sendHeader()
    {
        m_socket.async_send_to(
            asio::buffer(&m_sendQueue.front().header, sizeof(messageHeader<T>)),
            m_serverEndpoint,
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
                } else {
                    std::cerr << ec.message() << std::endl;
                }
            });
    }
    void readBody()
    {
        m_socket.async_receive_from(
            asio::buffer(m_readMessage.body.data(), m_readMessage.header.size),
            m_serverEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    uint32_t receivedChecksum = m_readMessage.header.checksum;
                    uint32_t calculatedChecksum = m_readMessage.calculateChecksum(
                        reinterpret_cast<const char *>(m_readMessage.body.data()),
                        m_readMessage.header.size);
                    if (receivedChecksum != calculatedChecksum)
                        return readHeader();
                    m_readQueue.push(m_readMessage);
                    readHeader();
                }
            });
    }

    void readHeader()
    {
        m_socket.async_receive_from(
            asio::buffer(&m_readMessage.header, sizeof(messageHeader<T>)),
            m_serverEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    if (m_readMessage.header.size > 0) {
                        m_readMessage.body.resize(m_readMessage.header.size);
                        readBody();
                    } else {
                        m_readQueue.push(m_readMessage);
                        readHeader();
                    }
                }
            });
    }

    asio::ip::udp::endpoint m_serverEndpoint;
    asio::ip::udp::socket m_socket;

    ThreadSafeQueue<asun::message<T>> m_readQueue;
    ThreadSafeQueue<asun::message<T>> m_sendQueue;

    message<T> m_readMessage{};
};
}  // namespace asun
