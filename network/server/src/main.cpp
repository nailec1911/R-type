/*
** EPITECH PROJECT, 2024
** src
** File description:
** main
*/

#include <csignal>
#ifdef __linux__
    #include <unistd.h>
#endif

#include <chrono>
#include <cstdint>
#include <exception>
#include <vector>

#include "../../../gameEngine/RTypeGame/RTypeGameServer.hpp"
#include "LevelConfigParser.hpp"
#include "RtypeServer.hpp"

int main(int argc, char **argv)
{
    try {
        checkParametersServer(argc, argv);
        LevelConfigParser levelParser;
        rtypeNetwork::RtypeServer server(static_cast<uint16_t>(std::stoi(argv[1])), 5); // NOLINT
        std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>>
        &levelOne = levelParser.getLevelbyId(1);
        gameEngine::RTypeGameServer rType;
        auto tickDuration = rtypeNetwork::RtypeServer::initTickRate(128);
        auto nextTick = chrono::now();
        rType.initGameRules();
        server.start();
        while (true) {
            auto start = chrono::now();
            {
                std::vector<uint32_t> playersToRemove{};
                rType.gameTrigger(levelOne.first);
                server.handleMessages();
                auto &clientsEvents = server.getClientsEvents();
                if (!rType.isGameDone()) {
                    auto snapshots =
                        rType.updateSystems(clientsEvents, playersToRemove);
                    server.setSnapshots(snapshots, playersToRemove);
                    rType.createFromConfig(levelOne.second);
                } else {
                    asun::message<CustomMessageType> msg{};
                    msg.header.id = CustomMessageType::WIN;
                    server.sendMessageToAllClients(msg);
                }
            }
            auto elapsedTime = chrono::now() - start;
            nextTick += tickDuration;
            if (elapsedTime < tickDuration) {
                std::this_thread::sleep_until(nextTick);
            } else {
                nextTick = chrono::now();
            }
        }
    } catch (const ErrorParams &p) {
        std::cerr << p.what() << std::endl;
        return 84;
    } catch (const LevelConfigParser::ErrorLevelParser &l) {
        std::cerr << l.what() << std::endl;
        return 84;
    } catch (const rtypeNetwork::RtypeServer::ErrorRtypeServer &s) {
        std::cerr << s.what() << std::endl;
        return 84;
    } catch (const HelpExceptionServer &e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &error) {
        std::cerr << "Error, exiting program..." << std::endl;
        return 84;
    }
    return 0;
}
