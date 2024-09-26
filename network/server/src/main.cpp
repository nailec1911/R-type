/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "RtypeServer.hpp"
#include "../../../gameEngine/RTypeGame.hpp"

#include <cstdint>
#include <queue>
#include <unordered_map>

int main()
{
    rtypeNetwork::RtypeServer server(4444, 5);
    gameEngine::RTypeGame rType;
    std::queue<rtypeNetwork::clientEvent> clientsEvents;
    rType.initGameRules();
    server.start();
    while (true)
    {
        sleep(1);
        server.handleMessages();
        clientsEvents = server.getClientsEvents();
        rType.getSystems().getInputsSystem()->Update(
            rType.getMediator(), clientsEvents);
        rType.getSystems().getMotionSystem()->Update(1, rType.getMediator());
        rType.getSystems().getCollisionSystem()->Update(rType.getMediator());
        std::unordered_map<uint32_t, SnapshotData> snapshots = rType.createSnapshots();
        server.sendMaster(snapshots, 1, 2);
    }
    return 0;
}
