/*
** EPITECH PROJECT, 2024
** rts
** File description:
** updateDataStruct
*/

#pragma once

#include <queue>

#include "Renderer/Events.hpp"
#include "ECS/using.hpp"

struct DataUpdate
{
    std::queue<clientEvent> clientsEvents;
    std::vector<uint32_t> deadPlayers;
    std::vector<Entity> entitiesToRemove;
    size_t nbPlayers;
};