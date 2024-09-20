/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpClientInterface
*/

#pragma once

#include "../../Message.hpp"

namespace network {
    class AsioUdpClientInterface {
    public:
        AsioUdpClientInterface() = default;
        AsioUdpClientInterface(AsioUdpClientInterface &&) = delete;
        AsioUdpClientInterface(const AsioUdpClientInterface &) = delete;
        AsioUdpClientInterface &operator=(AsioUdpClientInterface &&) = delete;
        AsioUdpClientInterface &operator=(const AsioUdpClientInterface &) = delete;
        virtual ~AsioUdpClientInterface() = default;

        virtual void sendMessage(message<network::messageType> &msg) = 0;
        virtual void readMessages() = 0;

    private:
    };
} // namespace network
