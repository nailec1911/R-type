/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RtypeServer
*/

#include "RtypeServer.hpp"

#include <asio/ip/udp.hpp>
#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "../../../gameEngine/RTypeGame/RTypeGameServer.hpp"

rtypeNetwork::RtypeServer::RtypeServer(uint16_t port, uint8_t maxClient)
    : m_maxClient(maxClient), m_gameServ(port), m_ready(false)
{
    m_tickRateThread = std::thread([this]() { this->manageTickRate(); });
}

rtypeNetwork::RtypeServer::~RtypeServer()
{
    if (m_tickRateThread.joinable())
        m_tickRateThread.join();
}

void rtypeNetwork::RtypeServer::manageTickRate()
{
    auto tickDuration = initTickRate(32);
    auto nextTick = chrono::now();
    while (true) {
        auto start = chrono::now();
        {
            std::unique_lock<std::mutex> mtx(m_mtx);
            m_cond.wait(mtx, [this]() { return m_ready; });
            while (!m_snapshots.empty()) {
                auto snap = m_snapshots.front();
                m_snapshots.pop();
                this->sendMaster(snap);
            }
            m_ready = false;
        }
        auto elapsedTime = chrono::now() - start;
        nextTick += tickDuration;
        if (elapsedTime < tickDuration) {
            std::this_thread::sleep_until(nextTick);
        } else {
            nextTick = chrono::now();
        }
    }
}

void rtypeNetwork::RtypeServer::handleMessages()
{
    auto msgQueue = m_gameServ.getMessages();
    while (!msgQueue.empty()) {
        auto msg = msgQueue.front();
        uint32_t clientId = msg.first;
        auto content = msg.second;
        msgQueue.pop();
        if (content.header.id == CustomMessageType::LOGIN) {
            handleClientLogin(clientId, content);
            continue;
        }
        if (content.header.id == CustomMessageType::LOGOUT) {
            handleClientLogout(clientId, content);
            continue;
        }
        if (!isClientConnected(clientId)) {
            std::cerr << "why is this client not connected" << clientId
                      << std::endl;
            continue;
        }
        if (content.header.id == CustomMessageType::LIST) {
            handleListRoom(clientId);
            continue;
        }
        if (content.header.id == CustomMessageType::CREATE) {
            handleCreateRoom(clientId, content);
            continue;
        }
        if (content.header.id == CustomMessageType::JOIN) {
            handleJoinRoom(clientId, content);
            continue;
        }
        auto clientRoom = std::find_if(
            m_rooms.begin(), m_rooms.end(),
            [clientId](
                std::pair<const uint32_t, std::unique_ptr<gameRoom>> &room) {
                return std::find(
                           room.second->getConnectedClient().begin(),
                           room.second->getConnectedClient().end(),
                           clientId) != room.second->getConnectedClient().end();
            });
        switch (content.header.id) {
            case CustomMessageType::MOVE:
                handleClientMove(
                    clientRoom->second->getClientsEvents(), clientId, content);
                break;
            case CustomMessageType::SHOOT:
                handleClientShoot(
                    clientRoom->second->getClientsEvents(), clientId);
                break;
            case CustomMessageType::BIG_SHOOT:
                handleClientBigShoot(
                    clientRoom->second->getClientsEvents(), clientId);
                break;
            case CustomMessageType::SNAP_OK:
                handleClientSnapOk(clientId, content);
                break;
            default:
                break;
        }
    }
}

std::vector<std::string> split(const std::string &str)
{
    std::vector<std::string> result{};
    std::istringstream iss(str);
    std::string word{};
    while (iss >> word)
        result.push_back(word);
    return result;
}

void rtypeNetwork::RtypeServer::handleCreateRoom(
    uint32_t clientId, asun::message<CustomMessageType> &msg)
{
    std::string msgContent{};
    asun::message<CustomMessageType> response{};
    std::vector<std::string> msgArgs{};

    if (m_nbRooms >= 4)
        return;
    msg >> msgContent;
    msgArgs = split(msgContent);
    int levelNumber = std::stoi(msgArgs.at(1)) - 1;
    bool isPublic = msgArgs.at(2) == "PUBLIC";
    std::string password(msgArgs.size() == 4 ? msgArgs.at(3) : "");
    auto level = levelParser.getLevelbyId(levelNumber);
    if (level.first == -1)
        return;
    m_rooms.insert(
        {m_nbRooms,
         std::make_unique<gameRoom>(
             *this, level, m_nbRooms, isPublic, password)});
    std::string message(
        "Room " + std::to_string(m_nbRooms) + " create with success");
    response << message;
    m_nbRooms += 1;
    response.header.id = CustomMessageType::CREATE;
    response.header.checksum = response.calculateChecksum(
        reinterpret_cast<const char *>(response.body.data()),
        response.header.size);
    sendMessageToClient(response, clientId);
    std::cout << "[create]" << std::endl;
}

void rtypeNetwork::RtypeServer::handleJoinRoom(
    uint32_t clientId, asun::message<CustomMessageType> &msg)
{
    asun::message<CustomMessageType> response{};
    std::string messageContent{};
    size_t roomId = 0;
    std::vector<std::string> msgArgs{};

    msg >> messageContent;
    msgArgs = split(messageContent);
    roomId = std::stoi(msgArgs.at(0));
    if (m_rooms.find(roomId) == m_rooms.end()) {
        std::cout << "Room not found" << std::endl;
        return;
    }
    response.header.id = CustomMessageType::JOIN;
    if (!m_rooms[roomId]->isPublic()) {
        if (msgArgs.size() != 2) {
            std::cout << "Password is missing" << std::endl;
            return;
        }
        if (m_rooms[roomId]->getPassword() != msgArgs.at(1)) {
            std::cout << "wrong password" << std::endl;
            return;
        }
    }
    this->m_rooms[roomId]->getConnectedClient().push_back(clientId);
    sendMessageToClient(response, clientId);
    std::cout << "[join]" << std::endl;
}

void rtypeNetwork::RtypeServer::handleListRoom(uint32_t clientId)
{
    asun::message<CustomMessageType> response{};
    response.header.id = CustomMessageType::LIST;
    std::string message{};
    if (m_nbRooms <= 0)
        message += "No rooms available.\n";
    else
        message += "Rooms list:\n";
    for (auto &room : m_rooms) {
        std::string roomStatus(room.second->isPublic() ? "PUBLIC" : "PRIVATE");
        message += "Room n°" + std::to_string(room.first) + ": " +
                   std::to_string(room.second->getConnectedClient().size()) +
                   " players connected. Status: " + roomStatus + "\n";
    }
    response << message;
    response.header.checksum = response.calculateChecksum(
        reinterpret_cast<const char *>(response.body.data()),
        response.header.size);
    sendMessageToClient(response, clientId);
    std::cout << "[list]" << std::endl;
}

void rtypeNetwork::RtypeServer::handleClientLogin(
    uint32_t clientId, asun::message<CustomMessageType> & /*msg*/)
{
    if (isClientConnected(clientId)) {
        std::cerr << "already logged in" << std::endl;
        return;
    }
    if (m_gameServ.getNbClient() < m_maxClient) {
        std::cout << "client added" << clientId << std::endl;
        m_connectedClients.emplace_back(clientId);
        return;
    }
    std::cerr << "There is already 5 players." << std::endl;
    m_gameServ.removeClient(clientId);
}

void rtypeNetwork::RtypeServer::handleClientLogout(
    uint32_t clientId, asun::message<CustomMessageType> & /*msg*/)
{
    m_gameServ.removeClient(clientId);
    auto newEnd = std::remove(
        m_connectedClients.begin(), m_connectedClients.end(), clientId);
    m_connectedClients.erase(newEnd, m_connectedClients.end());
    std::cerr << "[-] User disconnected" << std::endl;
}

void rtypeNetwork::RtypeServer::handleClientMove(
    std::queue<clientEvent> &events, uint32_t &clientId,
    asun::message<CustomMessageType> &msg)
{
    std::string input(reinterpret_cast<char *>(msg.body.data()));

    if (input == "1")
        events.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyUp, .state = EventState::KeyPressed}});
    if (input == "2")
        events.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyRight, .state = EventState::KeyPressed}});
    if (input == "3")
        events.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyDown, .state = EventState::KeyPressed}});
    if (input == "4")
        events.push(
            {.id = clientId,
             .event = Event{
                 .key = EventKey::KeyLeft, .state = EventState::KeyPressed}});
}

void rtypeNetwork::RtypeServer::handleClientShoot(
    std::queue<clientEvent> &events, uint32_t &clientId)
{
    events.push(
        {.id = clientId,
         .event =
             Event{.key = EventKey::KeyB, .state = EventState::KeyPressed}});
}

void rtypeNetwork::RtypeServer::handleClientBigShoot(
    std::queue<clientEvent> &events, uint32_t &clientId)
{
    events.push(
        {.id = clientId,
         .event = Event{
             .key = EventKey::KeySpace, .state = EventState::KeyPressed}});
}

void rtypeNetwork::RtypeServer::handleClientSnapOk(
    uint32_t clientId, asun::message<CustomMessageType> &msg)
{
    uint32_t snapId = 0;
    msg >> snapId;
    m_gameServ.acknowledgeSnapshot(clientId, snapId);
}

rtypeNetwork::RtypeServer::gameRoom::gameRoom(
    rtypeNetwork::RtypeServer &server,
    std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>>
        &level,
    int roomId, bool &isPublic, std::string &password)
    : m_roomId(roomId),
      m_password(password),
      m_level(std::move(level)),
      m_isPublic(isPublic)
{
    m_roomThread = std::thread(
        [this, &server]() { this->launchGame(server); });
}

rtypeNetwork::RtypeServer::gameRoom::~gameRoom()
{
    if (m_roomThread.joinable())
        m_roomThread.join();
}

void rtypeNetwork::RtypeServer::gameRoom::launchGame(
    rtypeNetwork::RtypeServer &server)
{
    gameEngine::RTypeGameServer rType;
    rType.initGameRules();

    auto tickDuration = initTickRate(32);
    auto nextTick = chrono::now();

    while (m_connectedClientsRoom.size() <= 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };
    while (true) {
        auto start = chrono::now();
        {
            std::vector<uint32_t> playersToRemove{};
            std::unordered_map<uint32_t, Entity> clientsIdByEntities{};
            rType.gameTrigger(m_level.first);
            auto &clientsEvents = getClientsEvents();
            if (!rType.isGameDone()) {
                auto snapshots = rType.updateSystems(
                    clientsEvents, playersToRemove, m_tick,
                    clientsIdByEntities);
                {
                    server.setSnapshots(
                        snapshots, playersToRemove, clientsIdByEntities, m_tick,
                        m_roomId);
                }
                m_tick += 1;
                rType.createFromConfig(m_level.second, m_tick);
            } else {
                asun::message<CustomMessageType> msg{};
                msg.header.id = CustomMessageType::WIN;
                for (auto &clientId : m_connectedClientsRoom)
                    server.sendMessageToClient(msg, clientId);
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
}
