/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeServer
*/

#pragma once

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../../../gameEngine/Renderer/Events.hpp"
#include "../../../gameEngine/Snapshot/SnapshotData.hpp"
#include "../../Errors.hpp"
#include "../../gameServ/GameServer.hpp"
#include "LevelConfigParser.hpp"

namespace rtypeNetwork {
class RtypeServer
{
   public:
    class gameRoom
    {
       public:
        gameRoom() = default;
        gameRoom(
            rtypeNetwork::RtypeServer &server,
            std::pair<
                float, std::unordered_map<float, std::vector<entitySpawn>>>
                &level,
            int roomId, bool &isPublic,
            std::string &password);
        ~gameRoom();
        gameRoom(const gameRoom &) = delete;
        gameRoom(gameRoom &&) noexcept = default;
        gameRoom &operator=(gameRoom &&) = default;
        gameRoom &operator=(gameRoom &) = delete;
        gameRoom &operator=(const gameRoom &) = delete;

        std::vector<uint32_t> &getConnectedClient()
        {
            return m_connectedClientsRoom;
        }

        std::queue<clientEvent> &getClientsEvents()
        {
            return this->m_clientsEvents;
        }

        bool isPublic() const
        {
            return m_isPublic;
        }

        std::string getPassword() const
        {
            return m_password;
        }

        void launchGame(
            rtypeNetwork::RtypeServer &server);

       private:
        int m_roomId{};
        std::string m_password;
        uint8_t m_maxClient{};
        std::thread m_roomThread;
        std::pair<float, std::unordered_map<float, std::vector<entitySpawn>>>
            m_level;
        std::queue<clientEvent> m_clientsEvents;
        std::vector<uint32_t> m_connectedClientsRoom;
        uint32_t m_tick{};
        bool m_isPublic{};
        bool m_ready{};
    };
    class ErrorRtypeServer : std::exception
    {
       public:
        ErrorRtypeServer(const std::string &msg) : m_msg(msg) {}

        const char *what() const noexcept override
        {
            return m_msg.c_str();
        }

       private:
        std::string m_msg;
    };
    RtypeServer(uint16_t port, uint8_t maxClient);
    RtypeServer(RtypeServer &&) = delete;
    RtypeServer(const RtypeServer &) = delete;
    RtypeServer &operator=(RtypeServer &&) = delete;
    RtypeServer &operator=(const RtypeServer &) = delete;
    ~RtypeServer();

    void handleMessages();

    void start()
    {
        m_gameServ.start();
    }

    void sendMaster(const std::pair<
                    std::pair<uint32_t, int>,
                    std::unordered_map<uint32_t, SnapshotData>> &eltsMap)
    {
        m_snapId++;
        uint32_t tick = eltsMap.first.first;
        int roomId = eltsMap.first.second;

        m_gameServ.sendMaster(
            CustomMessageType::SNAPSHOT,
            gameServer::Snapshot<SnapshotData, 1>(
                m_snapId, {static_cast<int>(tick)}, eltsMap.second),
            m_rooms[roomId]->getConnectedClient());
    }

    void setSnapshots(
        std::unordered_map<uint32_t, SnapshotData> &snapshots,
        std::vector<uint32_t> &playersToRemove,
        std::unordered_map<uint32_t, Entity> &clientsIdByEntities,
        uint32_t tick, int roomId)
    {
        for (auto &clientId : playersToRemove) {
            asun::message<CustomMessageType> msg{};
            msg.header.id = CustomMessageType::DEAD;
            sendMessageToClient(msg, clientId);
        }
        for (auto &clientsPlayers : clientsIdByEntities) {
            asun::message<CustomMessageType> msg{};
            msg.header.id = CustomMessageType::YOUR_PLAYER;
            msg << clientsPlayers.second;
            msg.header.checksum = msg.calculateChecksum(
                reinterpret_cast<const char *>(msg.body.data()),
                msg.header.size);
            sendMessageToClient(msg, clientsPlayers.first);
        }
        {
            this->m_snapshots.push({{tick, roomId}, snapshots});
            m_ready = true;
        }
        m_cond.notify_one();
    }

    void sendMessageToClient(
        asun::message<CustomMessageType> &msg, uint32_t clientId)
    {
        this->m_gameServ.sendMessageToClient(msg, clientId);
    }

    void sendMessageToAllClients(asun::message<CustomMessageType> &msg)
    {
        this->m_gameServ.sendMessageToAllClients(msg);
    }

    size_t getNbClientConnected() const
    {
        return m_connectedClients.size();
    }

   private:
    uint32_t m_snapId{};
    uint8_t m_maxClient;
    gameServer::GameServer<SnapshotData, 1, CustomMessageType> m_gameServ;
    std::vector<uint32_t> m_connectedClients;
    std::thread m_tickRateThread;
    std::condition_variable m_cond;
    std::unordered_map<uint32_t, std::unique_ptr<gameRoom>> m_rooms;
    std::mutex m_mtx;
    bool m_ready;
    std::queue<std::pair<
        std::pair<uint32_t, int>, std::unordered_map<uint32_t, SnapshotData>>>
        m_snapshots;
    int m_nbRooms{};
    LevelConfigParser levelParser;

    void handleJoinRoom(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleListRoom(uint32_t clientId);
    void handleCreateRoom(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientLogin(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientLogout(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientMove(
        std::queue<clientEvent> &events, uint32_t &clientId,
        asun::message<CustomMessageType> &msg);
    void handleClientShoot(std::queue<clientEvent> &events, uint32_t &clientId);
    void handleClientBigShoot(
        std::queue<clientEvent> &events, uint32_t &clientId);
    void handleClientSnapOk(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    bool isClientConnected(uint32_t clientId)
    {
        return std::find(
                   m_connectedClients.begin(), m_connectedClients.end(),
                   clientId) != m_connectedClients.end();
    }
    void manageTickRate();
};
}  // namespace rtypeNetwork
