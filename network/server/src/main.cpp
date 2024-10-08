/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include <unistd.h>

#include <chrono>

#include "../../../gameEngine/RTypeGame.hpp"
#include "RtypeServer.hpp"

int main()
{
    rtypeNetwork::RtypeServer server(4444, 5);
    gameEngine::RTypeGame rType;
    auto tickDuration = rtypeNetwork::RtypeServer::initTickRate();
    auto nextTick = chrono::now();

    rType.initGameRules();
    server.start();
    rType.createEntity(SHOOTER_MONSTER, {1700, 400});
    rType.createEntity(FLYING_MONSTER, {1700, 800});
    while (true) {
        auto start = chrono::now();
        {
            server.handleMessages();
            auto &clientsEvents = server.getClientsEvents();
            auto snapshots = rType.updateSystems(clientsEvents);
            server.setSnapshots(snapshots);
        }
        auto elapsedTime = chrono::now() - start;
        nextTick += tickDuration;
        if (elapsedTime < tickDuration) {
            std::this_thread::sleep_until(nextTick);
        } else {
            nextTick = chrono::now();
        }
    }
    return 0;
}
