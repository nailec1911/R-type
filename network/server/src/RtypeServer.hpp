/*
** EPITECH PROJECT, 2024
** src
** File description:
** RtypeServer
*/

#pragma once

#include <algorithm>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../../../gameEngine/Renderer/Events.hpp"
#include "../../../gameEngine/Snapshot/SnapshotData.hpp"
#include "../../gameServ/GameServer.hpp"
#include "../../Errors.hpp"
using chrono = std::chrono::steady_clock;

namespace rtypeNetwork {
class RtypeServer
{
   public:
    class ErrorRtypeServer : std::exception {
        public:
            ErrorRtypeServer(const std::string &msg) : m_msg(msg) {}
            
            const char* what() const noexcept override
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

    std::queue<clientEvent> &getClientsEvents()
    {
        return this->m_clientsEvents;
    }

    void start()
    {
        m_gameServ.start();
    }

    void sendMaster(
        const std::unordered_map<uint32_t, SnapshotData> &eltsMap, int other1,
        int other2)
    {
        m_snapId++;

        m_gameServ.sendMaster(
            CustomMessageType::SNAPSHOT,
            gameServer::Snapshot<SnapshotData, 2>(
                m_snapId, {other1, other2}, eltsMap));
    }

    void setSnapshots(
        std::unordered_map<uint32_t, SnapshotData> &snapshots,
        std::vector<uint32_t> &playersToRemove)
    {
        for (auto &clientId : playersToRemove) {
            asun::message<CustomMessageType> msg{};
            msg.header.id = CustomMessageType::DEAD;
            sendMessageToClient(msg, clientId);
        }
        {
            std::unique_lock<std::mutex> mtx(m_mtx);
            this->m_snapshots.push(snapshots);
            m_ready = true;
        }
        m_cond.notify_one();
    }

    static std::chrono::steady_clock::duration initTickRate(int tickRate)
    {
        auto tickDuration = std::chrono::duration_cast<chrono::duration>(
            std::chrono::duration<double>(1.0 / tickRate));
        return tickDuration;
    }

    void sendMessageToClient(
        asun::message<CustomMessageType> &msg, uint32_t clientId)
    {
        this->m_gameServ.sendMessageToClient(msg, clientId);
    }

        void sendMessageToAllClients(
        asun::message<CustomMessageType> &msg)
    {
        this->m_gameServ.sendMessageToAllClients(msg);
    }

   private:
    uint32_t m_snapId{0};
    uint8_t m_maxClient;
    std::queue<clientEvent> m_clientsEvents;
    gameServer::GameServer<SnapshotData, 2, CustomMessageType> m_gameServ;
    std::vector<uint32_t> m_connectedClients;
    std::thread m_tickRateThread;
    std::condition_variable m_cond;
    std::mutex m_mtx;
    bool m_ready;
    std::queue<std::unordered_map<uint32_t, SnapshotData>> m_snapshots;

    void handleClientLogin(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientLogout(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientMove(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
    void handleClientShoot(
        uint32_t clientId, asun::message<CustomMessageType> &msg);
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
