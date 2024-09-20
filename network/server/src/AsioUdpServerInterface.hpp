/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServerInterface
*/

#pragma once

#include <asio.hpp>

namespace network {
    class AsioUdpServerInterface {
    public:
        AsioUdpServerInterface() = default;
        AsioUdpServerInterface(AsioUdpServerInterface &&) = delete;
        AsioUdpServerInterface(const AsioUdpServerInterface &) = delete;
        AsioUdpServerInterface &operator=(AsioUdpServerInterface &&) = delete;
        AsioUdpServerInterface &operator=(const AsioUdpServerInterface &) = delete;
        virtual ~AsioUdpServerInterface() = default;

    protected:
        virtual void receiveClientsMessages() = 0;
        virtual void sendMessagesToClient() = 0;
        virtual void sendMessagesToAllClients() = 0;
    };
} //namespace network
