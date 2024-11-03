/*
** EPITECH PROJECT, 2024
** network
** File description:
** Message
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <type_traits>
#include <vector>

namespace asun {

template <typename T>
struct messageHeader
{
    T id{};
    std::uint32_t size = 0;
    std::uint32_t checksum{};
    bool reliable = false;
    std::uint32_t sequence = 0;
};

template <typename T>
struct message
{
    messageHeader<T> header{};
    std::vector<uint8_t> body{};

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

    friend message<T> &operator<<(message<T> &msg, const std::string &data)
    {
        size_t size = data.size();
        msg << size;
        std::size_t bodySize = msg.body.size();
        msg.body.resize(bodySize + size);
        std::memcpy(msg.body.data() + bodySize, data.data(), size);
        msg.header.size = msg.size();

        return msg;
    }

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

    friend message<T> &operator>>(message<T> &msg, std::string &data)
    {
        size_t strSize = 0;
        std::memcpy(&strSize, msg.body.data(), sizeof(std::size_t));

        if (msg.body.size() < sizeof(std::size_t) + strSize) {
            throw std::runtime_error("Data size mismatch.");
        }
        data.resize(strSize);
        std::memcpy(data.data(), msg.body.data() + sizeof(size_t), strSize);
        msg.body.erase(msg.body.begin(), msg.body.begin() + strSize);
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
