/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServer
*/

#pragma once

#include "../AsioUdpServerInterface.hpp"
#include "../../../ThreadSafeQueue.hpp"
#include "../../../Message.hpp"
#include "rtypeRfc.hpp"
#include <asio/executor_work_guard.hpp>
#include <asio/io_context.hpp>
#include <thread>

namespace rtypeNetwork {

    constexpr std::size_t BUFFER_SIZE = 1024;

    struct clientSession {
        std::string username;
        asio::ip::udp::endpoint endpoint;
        std::string sessionId;
        std::chrono::steady_clock::time_point last_heartbeat;
    };

    class rtypeUdpServer : public network::AsioUdpServerInterface<rtypeNetwork::rtypeMessageType> {
    public:
        rtypeUdpServer(std::uint32_t port);
        rtypeUdpServer(rtypeUdpServer &&) = delete;
        rtypeUdpServer(const rtypeUdpServer &) = delete;
        rtypeUdpServer &operator=(rtypeUdpServer &&) = delete;
        rtypeUdpServer &operator=(const rtypeUdpServer &) = delete;
        ~rtypeUdpServer() override;

    private:
        asio::io_context m_ctx;
        asio::ip::udp::socket m_socket;

        std::unordered_map<std::string, struct clientSession> m_clients;

        std::array<char, BUFFER_SIZE> m_readBuffer;
        ThreadSafeQueue<network::message<rtypeNetwork::rtypeMessageType>> m_readQueue;
        std::thread m_thread;
        asio::ip::udp::endpoint m_remoteEndpoint;
        std::unique_ptr<asio::io_context::work> m_work;

        void receiveClientsMessages() override;

        // TODO ->
        void readHeader();
        void readBody() {}
        void sendMessagesToAllClients(const network::message<rtypeNetwork::rtypeMessageType> &msg) override {}
        void sendMessagesToClient(const network::message<rtypeNetwork::rtypeMessageType> &msg) override {}
        void sendHeader() {}
        void sendBody() {}
    };
} //namespace rtypeNetwork
