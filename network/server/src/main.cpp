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
    LevelConfigParser levelParser("../configLevel1.yml");
    std::unordered_map<float, std::vector<entitySpawn>> &level1 = levelParser.getLevel();

    gameEngine::RTypeGame rType;
    auto tickDuration = rtypeNetwork::RtypeServer::initTickRate(128);
    auto nextTick = chrono::now();

    rType.initGameRules();
    server.start();
    rType.startGame(level1);
    while (true) {
        auto start = chrono::now();
        {
            server.handleMessages();
            auto &clientsEvents = server.getClientsEvents();
            auto snapshots = rType.updateSystems(clientsEvents);
            server.setSnapshots(snapshots);
            rType.createFromConfig(level1);
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
