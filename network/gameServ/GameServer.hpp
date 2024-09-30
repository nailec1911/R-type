/*
** EPITECH PROJECT, 2024
** rts
** File description:
** GameServer
*/

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../server/src/AsioUdpServer.hpp"
#include "Snapshot.hpp"

#pragma once

namespace gameServer {

using clientSession = struct clientSession
{
    std::string clientProtocol;
    asio::ip::udp::endpoint endpoint;
    std::chrono::steady_clock::time_point lastHeartBeat;
};

template <typename Telement, int nb_others>
class ClientHistory
{
   public:
    ClientHistory() : m_Snapshots({Snapshot<Telement, nb_others>()}) {};
    ClientHistory(const ClientHistory &) = delete;
    ClientHistory(ClientHistory &&) = delete;
    ClientHistory &operator=(const ClientHistory &) = default;
    ClientHistory &operator=(ClientHistory &&) = default;
    ~ClientHistory() = default;

    Snapshot<Telement, nb_others> getOldSnap(
        Snapshot<Telement, nb_others> &master)
    {
        auto snapVec = std::move(m_Snapshots);
        // m_Snapshots.erase();
        m_Snapshots.push_back(std::move(master));

        if (snapVec.empty())
            return  Snapshot<Telement, nb_others>();

        for (auto &snap : snapVec) {
            if (time(0) - snap.getCreationTime() > 1) {
                return Snapshot<Telement, nb_others>();
            }
            m_Snapshots.push_back(snap);
            if (snap.isAcknowledged()) {
                return snap;
            }
        }
        if (m_Snapshots.size() > 32) {
            m_Snapshots.resize(32);
        }
        return Snapshot<Telement, nb_others>();
    }

    void acknowledgeSnapshot(uint32_t idSnapshot)
    {
        for (auto &snap : m_Snapshots) {
            if (snap.getId() == idSnapshot) {
                snap.setAcknowledge();
                return;
            }
        }
    }

   private:
    std::vector<Snapshot<Telement, nb_others>> m_Snapshots;
};

template <typename Telement, int nb_others, typename Tmessage>
class GameServer : public asun::AsioUdpServer<Tmessage>
{
   public:
    GameServer(uint16_t port) : asun::AsioUdpServer<Tmessage>(port), m_master(Snapshot<Telement, nb_others>()){};
    GameServer(const GameServer &) = delete;
    GameServer(GameServer &&) = delete;
    GameServer &operator=(const GameServer &) = delete;
    GameServer &operator=(GameServer &&) = delete;
    ~GameServer() = default;

    void sendMaster(Tmessage headerId, Snapshot<Telement, nb_others> newSnapshot)
    {
        m_master = std::move(newSnapshot);
        for (auto &items : m_clientHistory) {
            sendClient(headerId, items.first);
        }
    };

    void acknowledgeSnapshot(uint32_t clientId, uint32_t snapId)
    {
        m_clientHistory[clientId].acknowledgeSnapshot(snapId);
    }

    std::queue<std::pair<uint32_t, asun::message<Tmessage>>> getMessages()
    {
        std::queue<std::pair<uint32_t, asun::message<Tmessage>>> res;

        while (this->getSizeReadQueue() > 0) {
            auto msg = this->popReadQueue();
            uint32_t clientId = getIdFromEndpoint(msg.first);
            if (clientId == 0) {
                clientId = handleClientLogin(msg);
            }
            res.push({clientId, msg.second});
        }
        return res;
    }

    uint32_t handleClientLogin(
        std::pair<asio::ip::udp::endpoint, asun::message<Tmessage>> &msg)
    {
        std::string endpointData = msg.first.address().to_string() + ":" +
                                   std::to_string(msg.first.port());
        for (auto &elem : m_clients) {
            if (elem.second.clientProtocol == endpointData) {
                std::cerr << "Client already connected..." << std::endl;
                return 0;
            }
        }
        m_lastId++;
        m_clients[m_lastId] = clientSession{
            .clientProtocol = endpointData,
            .endpoint = msg.first,
            .lastHeartBeat = std::chrono::steady_clock::now()};
        m_clientHistory[m_lastId] = ClientHistory<Telement, nb_others>();
        return m_lastId;
    }

    void removeClient(uint32_t clientId)
    {
        m_clientHistory.erase(clientId);
        m_clients.erase(clientId);
    }

    size_t getNbClient() const
    {
        return m_clients.size();
    };

   protected:
   private:
    uint32_t m_lastId{};

    std::unordered_map<uint32_t, clientSession> m_clients;
    std::unordered_map<uint32_t, ClientHistory<Telement, nb_others>>
        m_clientHistory;
    Snapshot<Telement, nb_others> m_master;

    uint32_t getIdFromEndpoint(asio::ip::udp::endpoint &endpoint)
    {
        uint16_t clientId = 0;
        std::string endpointData = endpoint.address().to_string() + ":" +
                                   std::to_string(endpoint.port());

        for (auto &elem : m_clients) {
            if (elem.second.clientProtocol == endpointData) {
                clientId = elem.first;
                break;
            }
        }
        return clientId;
    }

    void sendClient(Tmessage headerId, uint32_t clientId)
    {
        auto oldSnap = m_clientHistory[clientId].getOldSnap(m_master);

        sendDeltaDiff(headerId, clientId, oldSnap);
    };

    void sendDeltaDiff(Tmessage headerId,
        uint32_t clientId, Snapshot<Telement, nb_others> oldSnap)
    {
        std::array<int, nb_others> diffOthers{0};
        std::bitset<nb_others> updateOthers;
        auto masterOther = m_master.getOthers();
        auto oldOther = oldSnap.getOthers();

        for (int i = 0; i < nb_others; i += 1) {
            if (masterOther[i] != oldOther[i]) {
                diffOthers[i] = masterOther[i];
                updateOthers.set(i, true);
            }
        }

        std::unordered_map<uint32_t, Telement> diffElements;
        std::unordered_map<uint32_t, Telement> oldElements = oldSnap.getElements();
        std::unordered_map<uint32_t, Telement> masterElements = m_master.getElements();

        for (auto elt : oldElements) {
            if (elt.second != masterElements[elt.first]) {
                diffElements[elt.first] = masterElements[elt.first];
            }
        }

        auto deltaDiff = Snapshot<Telement, nb_others>(
            m_master.getId(), diffOthers, diffElements, updateOthers);
        asun::message<Tmessage> msg{};
        msg.header.id = headerId;
        msg << deltaDiff.toMessage();

        std::cout << "after overload" << std::endl;
        for (size_t i = 0; i < msg.body.size(); i++) {
            std::cout << "- "<< static_cast<int>(msg.body[i]) << std::endl;
        }

        std::cout << "deltaDiff: [" << deltaDiff.getId() << "]" << std::endl;
        std::cout << "others:" << deltaDiff.getOthers().at(0) << ' ' << deltaDiff.getOthers().at(1) << std::endl;
        std::cout << "nbElts:" << deltaDiff.getElements().size() << std::endl;
        std::cout << "master msg: " << msg << std::endl;
        this->sendMessage(m_clients[clientId].endpoint, msg);
    }
};
};  // namespace gameServer
