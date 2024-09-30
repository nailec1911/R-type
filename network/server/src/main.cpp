/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include <cstdint>
#include <unordered_map>

#include "../../../gameEngine/RTypeGame.hpp"
#include "RtypeServer.hpp"

int main()
{
    rtypeNetwork::RtypeServer server(4444, 5);
    gameEngine::RTypeGame rType;
    rType.initGameRules();
    server.start();
    while (true) {
        server.handleMessages();
        auto &clientsEvents = server.getClientsEvents();
        rType.getSystems().getInputsSystem()->Update(
            rType.getMediator(), clientsEvents);
        rType.getSystems().getMotionSystem()->Update(1, rType.getMediator());
        rType.getSystems().getCollisionSystem()->Update(rType.getMediator());
        std::unordered_map<uint32_t, SnapshotData> snapshots =
            rType.createSnapshots();
        server.sendMaster(snapshots, 1, 2);
        usleep(10000);
    }
    return 0;
}
