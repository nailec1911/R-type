/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#pragma once

#include <asio.hpp>
#include <cstdint>
#include <iostream>

#include "../../Message.hpp"
#include "../../ThreadSafeQueue.hpp"

namespace asun {

class AsioNetworkThread
{
   public:
    AsioNetworkThread(uint16_t port)
        : m_guard(asio::make_work_guard(m_ctx)), m_port(port)
    {
    }
    AsioNetworkThread(AsioNetworkThread &&) = delete;
    AsioNetworkThread(const AsioNetworkThread &) = delete;
    AsioNetworkThread &operator=(AsioNetworkThread &&) = delete;
    AsioNetworkThread &operator=(const AsioNetworkThread &) = delete;
    virtual ~AsioNetworkThread()
    {
        stop();
    }

    void start()
    {
        m_iothread = std::thread([this]() { m_ctx.run(); });
        std::cout << "[+]Server started !" << std::endl;
    }

    void stop()
    {
        m_ctx.stop();
        if (m_iothread.joinable()) {
            m_iothread.join();
        }
        std::cout << "[-]Server stoped !" << std::endl;
    }

   protected:
    asio::io_context m_ctx;  // NOLINT
    asio::executor_work_guard<asio::io_context::executor_type>
        m_guard;             // NOLINT
    std::thread m_iothread;  // NOLINT
    uint16_t m_port;         // NOLINT
};

template <typename T>
class AsioUdpServer : public AsioNetworkThread
{
   public:
    AsioUdpServer(uint16_t port)
        : AsioNetworkThread(port),
          m_socket(m_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
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

    virtual void handleMessages() = 0;

   protected:
    void sendMessage(
        const asio::ip::udp::endpoint &endpoint, const message<T> &msg)
    {
        m_socket.async_send_to(
            asio::buffer(&msg.header, sizeof(messageHeader<T>)), endpoint,
            [this](std::error_code ec) {
                if (!ec) {
                    std::cout << "Sucess\n";
                }
            });
    }

   private:
    void readBody()
    {
        m_socket.async_receive_from(
            asio::buffer(m_readMessage.body.data(), m_readMessage.body.size()),
            m_remoteEndpoint,
            [this](std::error_code ec, [[maybe_unused]] std::size_t length) {
                if (!ec) {
                    m_readQueue.push(m_readMessage);
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
                        std::cerr << "[-]Header error" << std::endl;
                    }
                }
            });
    }

    asio::ip::udp::socket m_socket;
    asio::ip::udp::endpoint m_remoteEndpoint;

    ThreadSafeQueue<asun::message<T>> m_readQueue;
    ThreadSafeQueue<asun::message<T>> m_sendQueue;

    message<T> m_readMessage{};
};
}  // namespace asun
