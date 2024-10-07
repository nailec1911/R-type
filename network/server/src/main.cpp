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
    auto tickDuration = server.initTickRate();
    auto nextTick = chrono::now();

    rType.initGameRules();
    server.start();
    Entity monstre = rType.getMediator()->CreateEntity();
    rType.getMediator()->AddComponent<Position>(monstre, Position{800, 800});
    rType.getMediator()->AddComponent<Transform>(monstre, Transform{0, 0});
    rType.getMediator()->AddComponent<BoundingBox>(monstre, BoundingBox{50, 60});
    rType.getMediator()->AddComponent<Monster>(monstre, Monster{});
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
