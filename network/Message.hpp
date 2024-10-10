/*
** EPITECH PROJECT, 2024
** network
** File description:
** Message
*/

#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <type_traits>
#include <vector>

namespace asun {

template <typename T>
struct messageHeader
{
    T id{};
    std::uint32_t size = 0;
    uint32_t checksum;
};

template <typename T>
struct message
{
    messageHeader<T> header{};
    std::vector<std::uint8_t> body{};

    [[nodiscard]] std::size_t size() const
    {
        return body.size();
    }

    friend std::ostream &operator<<(std::ostream &os, const message<T> &msg)
    {
        os << "ID: " << static_cast<int>(msg.header.id)
           << " Size: " << msg.header.size << std::endl;
        return os;
    };

    template <typename DataType>
    friend message<T> &operator<<(message<T> &msg, const DataType &data)
    {
        static_assert(
            std::is_standard_layout<DataType>::value, "Data too complex.");
        std::size_t bodySize = msg.body.size();
        msg.body.resize(bodySize + sizeof(DataType));
        std::memcpy(msg.body.data() + bodySize, &data, sizeof(DataType));
        msg.header.size = msg.size();

        return msg;
    }

    friend message<T> &operator<<(
        message<T> &msg, const std::vector<uint8_t> &data)
    {
        std::size_t bodySize = msg.body.size();
        msg.body.resize(bodySize + data.size());  // Resize to fit the new data
        std::memcpy(
            msg.body.data() + bodySize, data.data(),
            data.size());  // Copy the vector contents
        msg.header.size = msg.size();

        return msg;
    }

    template <typename DataType>
    friend message<T> &operator>>(message<T> &msg, DataType &data)
    {
        static_assert(
            std::is_standard_layout<DataType>::value, "Data too complex.");
        std::size_t bodySize = msg.body.size();
        std::memcpy(&data, msg.body.data(), bodySize);
        msg.body.resize(bodySize);
        msg.header.size = msg.size();

        return msg;
    }

    uint32_t calculateChecksum(const char *data, size_t length)
    {
        uint32_t checksum = 0;
        for (size_t i = 0; i < length; ++i) {
            checksum += data[i];
        }
        return checksum;
    }
};
}  // namespace asun
