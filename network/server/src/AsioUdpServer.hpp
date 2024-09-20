/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#pragma once

#include "AsioUdpServerInterface.hpp"
#include "../../ThreadSafeQueue.hpp"
#include "../../Message.hpp"
#include <asio/executor_work_guard.hpp>
#include <asio/io_context.hpp>
#include <thread>

namespace network {

    constexpr std::size_t BUFFER_SIZE = 1024;

    struct clientSession {
        std::string username;
        asio::ip::udp::endpoint endpoint;
        std::string sessionId;
        std::chrono::steady_clock::time_point last_heartbeat;
    };

    class AsioUdpServer : public AsioUdpServerInterface {
    public:
        AsioUdpServer(std::uint32_t port);
        AsioUdpServer(AsioUdpServer &&) = delete;
        AsioUdpServer(const AsioUdpServer &) = delete;
        AsioUdpServer &operator=(AsioUdpServer &&) = delete;
        AsioUdpServer &operator=(const AsioUdpServer &) = delete;
        ~AsioUdpServer() override;

    private:
        asio::io_context m_ctx;
        asio::ip::udp::socket m_socket;

        std::unordered_map<std::string, struct clientSession> m_clients;

        std::array<char, BUFFER_SIZE> m_readBuffer;
        ThreadSafeQueue<message<network::messageType>> m_readQueue;
        std::thread m_thread;
        asio::ip::udp::endpoint m_remoteEndpoint;
        std::unique_ptr<asio::io_context::work> m_work;

        void receiveClientsMessages() override;
        void sendMessagesToAllClients() override {
        }
        void sendMessagesToClient() override {
        }
    };
} //namespace network
