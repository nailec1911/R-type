/*
** EPITECH PROJECT, 2024
** src
** File description:
** rtypeRfc
*/

#pragma once

#include <cstdint>
namespace rtypeNetwork {
enum class rtypeMessageType : uint8_t
{
    SHOOT,
    MOVE,
    OK,
    KO,
    LOGIN,
};
}  // namespace rtypeNetwork
