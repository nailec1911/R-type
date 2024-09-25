/*
** EPITECH PROJECT, 2024
** src
** File description:
** AsioUdpServerInterface
*/

#pragma once

#include <asio.hpp>
#include <cstdint>

#include "../../Message.hpp"

namespace network {
template <typename T>
class AsioUdpServerInterface
{
   public:
    AsioUdpServerInterface() = default;
    AsioUdpServerInterface(AsioUdpServerInterface &&) = delete;
    AsioUdpServerInterface(const AsioUdpServerInterface &) = delete;
    AsioUdpServerInterface &operator=(AsioUdpServerInterface &&) = delete;
    AsioUdpServerInterface &operator=(const AsioUdpServerInterface &) = delete;
    virtual ~AsioUdpServerInterface() = default;

   protected:
    virtual void sendMessagesToClient(
        const message<T> &, uint16_t clientId) = 0;
    virtual void sendMessagesToAllClients(const message<T> &) = 0;
};
}  // namespace network
