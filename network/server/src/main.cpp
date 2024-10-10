/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <vector>

#include "../../../gameEngine/RTypeGame.hpp"
#include "RtypeServer.hpp"

int main(int argc, char **argv)
{
    if (!checkParamsServer(argc, argv))
        return 84;
    rtypeNetwork::RtypeServer server(std::stoi(argv[1]), 5);
    LevelConfigParser levelParser("../configLevel1.yml");
    std::unordered_map<float, std::vector<entitySpawn>> &level1 =
        levelParser.getLevel();

    gameEngine::RTypeGame rType;
    auto tickDuration = rtypeNetwork::RtypeServer::initTickRate(128);
    auto nextTick = chrono::now();

    rType.initGameRules();
    server.start();
    while (true) {
        auto start = chrono::now();
        {
            std::vector<uint32_t> playersToRemove{};
            rType.gameTrigger();
            server.handleMessages();
            auto &clientsEvents = server.getClientsEvents();
            auto snapshots = rType.updateSystems(clientsEvents, playersToRemove);
            server.setSnapshots(snapshots, playersToRemove);
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
