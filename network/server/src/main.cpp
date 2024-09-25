/*
** EPITECH PROJECT, 2024
** server
** File description:
** main
*/

#include <unistd.h>

#include <vector>

#include "../../../gameEngine/RTypeGame.hpp"
#include "rtypeServer/rtypeUdpServer.hpp"

int main()
{
    rtypeNetwork::rtypeUdpServer server(4444);
    gameEngine::RTypeGame rType;
    std::queue<clientEvent> clientsEvents;
    rType.initGameRules();

    while (true) {
        sleep(2);
        clientsEvents = server.getClientsEvents();
        rType.getSystems().getInputsSystem()->Update(
            rType.getMediator(), clientsEvents);
        rType.getSystems().getMotionSystem()->Update(1, rType.getMediator());
        rType.getSystems().getCollisionSystem()->Update(rType.getMediator());
        std::vector<SnapshotData> snapshots = rType.createSnapshots();
    }
    return 0;
}
