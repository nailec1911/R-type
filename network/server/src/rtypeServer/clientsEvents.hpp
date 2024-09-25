/*
** EPITECH PROJECT, 2024
** rts
** File description:
** clientsEvents
*/

#pragma once
#include <cstdint>

#include "../../../../gameEngine/Renderer/Events.hpp"

struct clientEvent
{
    uint16_t id;
    Event event;
};