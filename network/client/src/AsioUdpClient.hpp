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
    AsioNetworkThread() : m_guard(asio::make_work_guard(m_ctx)) {}
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
        std::cout << "[+]Client started !" << std::endl;
    }

    void stop()
    {
        m_ctx.stop();
        if (m_iothread.joinable()) {
            m_iothread.join();
        }
        std::cout << "[-]Client stoped !" << std::endl;
    }

   protected:
    asio::io_context m_ctx;  // NOLINT
    asio::executor_work_guard<asio::io_context::executor_type>
        m_guard;             // NOLINT
    std::thread m_iothread;  // NOLINT
};

template <typename T>
class AsioUdpClient : public AsioNetworkThread
{
   public:
    AsioUdpClient(const std::string &ip, uint16_t port)
        : AsioNetworkThread(),
          m_serverEndpoint(asio::ip::address::from_string(ip), port),
          m_socket(m_ctx, m_serverEndpoint.protocol())
    {
        m_socket.connect(m_serverEndpoint);
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

    virtual void handleMessages() = 0;

    // protected:
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
                        readBody();
                    } else {
                        m_readQueue.push(m_readMessage);
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
