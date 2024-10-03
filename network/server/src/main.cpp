/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include "../../../gameEngine/RTypeGame.hpp"
#include "RtypeServer.hpp"

#include <unistd.h>

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
        rType.getSystems().getMotionSystem()->Update(rType.getMediator());
        rType.getSystems().getCollisionSystem()->Update(rType.getMediator());
        rType.getSystems().getDestroyBulletSystem()->Update(rType.getMediator());
        auto snapshots = rType.createSnapshots();
        server.setSnapshots(snapshots);
    }
    return 0;
}
