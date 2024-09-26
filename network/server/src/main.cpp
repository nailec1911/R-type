/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "RtypeServer.hpp"
#include "../../../gameEngine/RTypeGame.hpp"

#include <queue>

int main()
{
    rtypeNetwork::RtypeServer server(4444);
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
        std::vector<SnapshotData> snapshots = rType.createSnapshots();
    }
    return 0;
}
