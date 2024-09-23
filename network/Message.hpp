/*
** EPITECH PROJECT, 2024
** network
** File description:
** Message
*/

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <ostream>
#include <type_traits>
#include <vector>

namespace network {

template <typename T>
struct messageHeader {
    T id{};
    std::uint32_t size = 0;
};

template <typename T>
struct message {
    messageHeader<T> header{};
    std::vector<std::uint8_t> body{};

    [[nodiscard]] std::size_t size() const {
        return sizeof(messageHeader<T>) + body.size();
    }

    friend std::ostream &operator<<(std::ostream &os, const message<T> &msg) {
        os << "ID: " << static_cast<uint8_t>(msg.header.id)
           << " Size: " << msg.header.size << " Content: " << msg.body.data()
           << std::endl;
        return os;
    };

    friend message<T> &operator<<(message<T> &dest, const message<T> &source) {
        // TODO
    }

    template <typename DataType>
    friend message<T> &operator<<(message<T> &msg, const DataType &data) {
        static_assert(std::is_standard_layout<DataType>::value,
                      "Data too complex.");
        std::size_t bodySize = msg.body.size();
        msg.body.resize(bodySize + sizeof(DataType));
        std::memcpy(msg.body.data() + bodySize, &data, sizeof(DataType));
        msg.header.size = msg.size();

        return msg;
    }

    template <typename DataType>
    friend message<T> &operator>>(message<T> &msg, DataType &data) {
        static_assert(std::is_standard_layout<DataType>::value,
                      "Data too complex.");
        std::size_t bodySize = msg.body.size();
        std::memcpy(&data, msg.body.data(), bodySize);
        msg.body.resize(bodySize);
        msg.header.size = msg.size();

        return msg;
    }
};
}  // namespace network
