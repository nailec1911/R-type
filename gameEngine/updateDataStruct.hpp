/*
** EPITECH PROJECT, 2024
** rts
** File description:
** updateDataStruct
*/

#pragma once

#include <cstdint>
#include <queue>
#include <unordered_map>
#include <vector>

#include "ECS/using.hpp"
#include "Renderer/Events.hpp"

struct DataUpdate
{
    std::queue<clientEvent> clientsEvents;
    std::vector<uint32_t> deadPlayers;
    std::vector<Entity> entitiesToRemove;
    size_t nbPlayers;
    uint32_t tick;
    std::unordered_map<uint32_t, Entity> clientsIdByEntities;
    std::vector<Event> events;
    int playerEntityId;
};
