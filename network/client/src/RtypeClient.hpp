/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeClient
*/

#pragma once

#include <cstdint>
#include <unordered_map>
#include "AsioUdpClient.hpp"

namespace rtypeNetwork {

enum class CustomMessageType
{
    OK,
    KO,
};

class RtypeClient : public asun::AsioUdpClient<CustomMessageType>
{
   public:
    RtypeClient(const std::string &ip, uint16_t port)
        : asun::AsioUdpClient<CustomMessageType>(ip, port)
    {}
    RtypeClient(RtypeClient &&) = delete;
    RtypeClient(const RtypeClient &) = delete;
    RtypeClient &operator=(RtypeClient &&) = delete;
    RtypeClient &operator=(const RtypeClient &) = delete;
    ~RtypeClient() override = default;

    void handleMessages() override {
    }

   private:
};
}  // namespace rtypeNetwork
