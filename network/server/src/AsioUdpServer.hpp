/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#pragma once

#include <cstddef>
#include <cstdint>

#include "../../AsioNetworkThread.hpp"
#include "../../Message.hpp"
#include "../../ThreadSafeQueue.hpp"

namespace asun {

template <typename T>
class AsioUdpServer : public AsioNetworkThread
{
   public:
    AsioUdpServer(uint16_t port)
        : AsioNetworkThread(port),
          m_socket(m_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
        m_socket.set_option(asio::socket_base::send_buffer_size(65536));
        readHeader();
    }
    AsioUdpServer(AsioUdpServer &&) = delete;
    AsioUdpServer(const AsioUdpServer &) = delete;
    AsioUdpServer &operator=(AsioUdpServer &&) = delete;
    AsioUdpServer &operator=(const AsioUdpServer &) = delete;
    ~AsioUdpServer() override
    {
        AsioNetworkThread::stop();
    }

   protected:
    void sendMessage(
        const asio::ip::udp::endpoint &endpoint, const message<T> &msg)
    {
        asio::post(
            m_ctx, [this, endpoint, msg]() { sendHeader(endpoint, msg); });
    }

    std::pair<asio::ip::udp::endpoint, asun::message<T>> popReadQueue()
    {
        return m_readQueue.pop();
    }

    [[nodiscard]] size_t getSizeReadQueue() const
    {
        return m_readQueue.size();
    }

   private:
    void sendBody(
        const asio::ip::udp::endpoint &clientEndpoint,
        const asun::message<T> &msg)
    {
        m_socket.async_send_to(
            asio::buffer(msg.body, msg.body.size()), clientEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                } else {
                    std::cerr << ec.message() << std::endl;
                }
            });
    }

    void sendHeader(
        const asio::ip::udp::endpoint &clientEndpoint,
        const asun::message<T> &msg)
    {
        m_socket.async_send_to(
            asio::buffer(&msg.header, sizeof(messageHeader<T>)), clientEndpoint,
            [this, msg, clientEndpoint](
                std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    if (msg.header.size > 0) {
                        sendBody(clientEndpoint, msg);
                    }
                } else {
                    std::cerr << ec.message() << std::endl;
                }
            });
    }

    void readBody()
    {
        m_socket.async_receive_from(
            asio::buffer(m_readMessage.body.data(), m_readMessage.body.size()),
            m_remoteEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    m_readQueue.push({m_remoteEndpoint, m_readMessage});
                    readHeader();
                } else {
                    std::cerr << "[-]Body error" << std::endl;
                }
            });
    }

    void readHeader()
    {
        m_socket.async_receive_from(
            asio::buffer(&m_readMessage.header, sizeof(asun::messageHeader<T>)),
            m_remoteEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    if (m_readMessage.header.size > 0) {
                        m_readMessage.body.resize(m_readMessage.header.size);
                        readBody();
                    } else {
                        m_readQueue.push({m_remoteEndpoint, m_readMessage});
                        readHeader();
                    }
                }
            });
    }

    asio::ip::udp::socket m_socket;
    asio::ip::udp::endpoint m_remoteEndpoint;

    ThreadSafeQueue<std::pair<asio::ip::udp::endpoint, asun::message<T>>>
        m_readQueue;
    ThreadSafeQueue<asun::message<T>> m_sendQueue;

    message<T> m_readMessage{};
};
}  // namespace asun
