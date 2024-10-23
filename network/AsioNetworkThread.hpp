/*
** EPITECH PROJECT, 2024
** network
** File description:
** AsioNetworkThread
*/

#pragma once

#include <asio.hpp>
#include <iostream>

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
        std::cout << "[+]Context running !\n";
    }

    void stop()
    {
        m_ctx.stop();
        if (m_iothread.joinable()) {
            m_iothread.join();
        }
        std::cout << "[-]Context stoped !\n";
    }

   protected:
    asio::io_context m_ctx;  // NOLINT
    asio::executor_work_guard<asio::io_context::executor_type>
        m_guard;             // NOLINT
    std::thread m_iothread;  // NOLINT
    uint16_t m_port;         // NOLINT
};
}  // namespace asun
