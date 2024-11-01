/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeClient
*/

#include "RtypeClient.hpp"

#include <fcntl.h>
#include <sys/select.h>

#include <cstdint>
#include <format>
#include <vector>

#include "../../../gameEngine/ECS/Managers/Component/StructComponent.hpp"

void rtypeNetwork::RtypeClient::handleMessages(
    gameEngine::RTypeGameClient &rType, std::vector<Entity> &entitiesToRemove)
{
    while (getSizeReadQueue() > 0) {
        auto msg = popReadQueue();
        if (msg.header.id == CustomMessageType::SNAPSHOT) {
            try {
                uint32_t id = updateGameData(
                    gameServer::Snapshot<SnapshotData, 1>(msg.body), rType,
                    entitiesToRemove);
                asun::message<CustomMessageType> ok;
                ok.header.id = CustomMessageType::SNAP_OK;
                ok << id;
                sendMessage(ok);
            } catch (const std::format_error &e) {
                std::cerr << e.what() << std::endl;
                return;
            }
        }
        if (msg.header.id == CustomMessageType::DEAD)
            m_isPlayerDead = true;
        if (msg.header.id == CustomMessageType::WIN && !m_isPlayerDead)
            m_hasPlayerWon = true;
        if (msg.header.id == CustomMessageType::YOUR_PLAYER) {
            Entity playerId = 0;
            msg >> playerId;
            setPlayerEntityId(playerId);
        }
    }
};

void rtypeNetwork::RtypeClient::handleMessages(bool &isChoosing)
{
    while (getSizeReadQueue() > 0) {
        auto msg = popReadQueue();
        if (msg.header.id == CustomMessageType::LIST) {
            std::string msgContent{};
            msg >> msgContent;
            std::cout << msgContent << std::endl;
        }
        if (msg.header.id == CustomMessageType::JOIN) {
            std::cout << "The player has been associate to the room"
                      << std::endl;
            isChoosing = false;
            continue;
        }
        if (msg.header.id == CustomMessageType::CREATE) {
            std::string message(msg.body.begin(), msg.body.end());
            std::cout << "[room created]" << std::endl;
        }
    }
}

static std::vector<std::string> parseInput(const std::string &input)
{
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

void rtypeNetwork::RtypeClient::listInfosServer()
{
    asun::message<CustomMessageType> listMsg;

    listMsg.header.id = CustomMessageType::LIST;
    sendMessage(listMsg);
}

void rtypeNetwork::RtypeClient::help()
{
    std::cout << "Usages : " << std::endl;
    std::cout << "      list : List all the rooms" << std::endl;
    std::cout << "      join [ID_ROOM] (PASSWORD): Join the room specified"
              << std::endl;
    std::cout
        << "      create [NB_PLAYERS] [LEVEL] [PRIVATE|PUBLIC] (PASSWORD): Create a room"
        << std::endl;
    std::cout << "      help : Show the usage" << std::endl;
}

void rtypeNetwork::RtypeClient::joinRoom(const std::vector<std::string> &argv)
{
    asun::message<CustomMessageType> joinMsg{};

    if (argv.size() < 2 || argv.size() > 3) {
        help();
        return;
    }
    joinMsg.header.id = CustomMessageType::JOIN;
    std::string password(argv.size() == 3 ? " " + argv.at(2) : "");
    joinMsg << std::string(argv.at(1) + password);
    sendMessage(joinMsg);
}

void rtypeNetwork::RtypeClient::createRoom(const std::vector<std::string> &argv)
{
    asun::message<CustomMessageType> createMsg{};

    if (argv.size() != 5 && argv.size() != 4) {
        help();
        return;
    }
    createMsg.header.id = CustomMessageType::CREATE;
    if (argv.at(3) == "PRIVATE" && argv.size() == 4) {
        std::cout << "You have to set a password for your private room."
                  << std::endl;
        return;
    }
    std::string password(argv.size() == 5 ? " " + argv.at(4) : "");
    createMsg << std::string(
        argv.at(1) + " " + argv.at(2) + " " + argv.at(3) + password);
    sendMessage(createMsg);
}

void rtypeNetwork::RtypeClient::serverInfos()
{
    bool isChoosing = true;
    std::string input;
    char buffer[256];
    int stdin_fd = fileno(stdin);
    int flags = fcntl(stdin_fd, F_GETFL, 0);
    fcntl(stdin_fd, F_SETFL, flags | O_NONBLOCK);

    while (isChoosing) {
        handleMessages(isChoosing);
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        struct timeval timeout
        {};
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ret = select(stdin_fd + 1, &read_fds, nullptr, nullptr, &timeout);
        if (ret > 0 && FD_ISSET(stdin_fd, &read_fds)) {
            ssize_t bytesRead = read(stdin_fd, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                input = std::string(buffer);
                auto argv = parseInput(input);
                if (argv.empty())
                    continue;
                const std::string &command = argv[0];
                if (command == "list")
                    listInfosServer();
                else if (command == "help")
                    help();
                else if (command == "create")
                    createRoom(argv);
                else if (command == "join")
                    joinRoom(argv);
                else if (command == "exit")
                    isChoosing = false;
                else
                    std::cout << "Unknown command: " << command << "\n";
            }
        }
    }
    fcntl(stdin_fd, F_SETFL, flags);
}

uint32_t rtypeNetwork::RtypeClient::updateGameData(
    const gameServer::Snapshot<SnapshotData, 1> &newSnapshot,
    gameEngine::RTypeGameClient &rType, std::vector<Entity> &entitiesToRemove)
{
    auto entities = rType.getMediator()->GetEntitiesSignatures();

    uint32_t tick = newSnapshot.getOthers()[0];

    for (auto item : newSnapshot.getElements()) {
        if (item.second.getType() == DESTROY) {
            rType.getMediator()->DestroyEntity(item.first);
            entitiesToRemove.push_back(item.first);
            continue;
        }
        auto type = item.second.getType();
        auto pos = item.second.getXY();
        auto vel = item.second.getVelocity();

        pos.first +=
            static_cast<int>(vel.first * (tick - item.second.getTick())) * 1 /
            32;
        pos.second +=
            static_cast<int>(vel.second * (tick - item.second.getTick())) * 1 /
            32;
        if (entities.find(item.first) == entities.end()) {
            if (type == PLAYER)
                rType.getNbPlayers() += 1;
            rType.createEntity(
                type, {pos.first, pos.second}, static_cast<int>(item.first),
                item.second.getTick());
            continue;
        }
        if (type == WALL || type == SHOOTER_MONSTER)
            continue;
        auto &position =
            rType.getMediator()->GetComponent<Position>(item.first);
        position.x = static_cast<float>(pos.first);
        position.y = static_cast<float>(pos.second);
        auto &velocity =
            rType.getMediator()->GetComponent<Transform>(item.first);
        velocity.velX = vel.first;
        velocity.velY = vel.second;
    }
    return newSnapshot.getId();
}
