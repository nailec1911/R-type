/*
** EPITECH PROJECT, 2024
** rts
** File description:
** GameServer
*/

#include "Snapshot.hpp"
#include <algorithm>
#include <array>
#include <bitset>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

#pragma once

template<typename Telement, typename Tother, int nb_others>
class ClientHistory {
public:
    ClientHistory();
    ~ClientHistory();

    Snapshot<Telement, Tother, nb_others> getOldSnap(Snapshot<Telement, Tother, nb_others> &master)
    {
        auto snapVec = m_Snapshots;
        m_Snapshots = {std::copy(master)};


        for (auto &snap : snapVec) {
            if (time(0) - snap.getCreationTime() > 1) {
                return Snapshot<Telement, Tother, nb_others>();
            }
            m_Snapshots.push_back(snap);
            if (snap.isAcknowledged()) {
                return snap;
            }
        }
        if (m_Snapshots.size() > 32) {
            m_Snapshots.resize(32);
        }
        return Snapshot<Telement, Tother, nb_others>();
    }

    void acknowledgeSnapshot(int idSnapshot) {
        for (auto &snap : m_Snapshots) {
            if (snap.getId() == idSnapshot) {
                snap.setAcknowledge();
                return;
            }
        }
    }

private:
    std::vector<Snapshot<Telement, Tother, nb_others>> m_Snapshots;
};


template<typename Telement, typename Tother, int nb_others>
class GameServer {
public:
    GameServer();
    ~GameServer();

    void sendMaster(Snapshot<Telement, Tother, nb_others> newSnapshot)
    {
        m_master = newSnapshot;
        for (auto &items : m_clientHistory) {
            sendClient(items.first);
        }
    };

protected:
private:
    void acknowledgeSnapshot(int clientId, int snapId)
    {
        m_clientHistory[clientId].acknowledgeSnapshot(snapId);
    }

    void sendClient(int clientId) {
        auto oldSnap = m_clientHistory[clientId].getOldSnap(m_master);

        sendDeltaDiff(clientId, oldSnap);
    };

    void sendDeltaDiff(int clientId, Snapshot<Telement, Tother, nb_others> oldSnap)
    {
        std::array<Tother, nb_others> diffOthers {0};
        std::bitset<nb_others> updateOthers;
        auto masterOther = m_master.getOthers();
        auto oldOther = oldSnap.getOthers();

        for (int i = 0; i < nb_others; i += 1) {
            if (masterOther[i] != oldOther[i]) {
                diffOthers[i] = masterOther[i];
                updateOthers.set(i, true);
            }
        }


        std::map<u_int32_t, Telement> diffElements;
        std::map<u_int32_t, Telement> &oldElements = oldSnap.getElements();
        std::map<u_int32_t, Telement> &masterElements = m_master.getElements();

        for (auto &elt : oldOther) {
            if (elt.second != masterElements[elt.first]) {
                diffElements[elt.first] = masterElements[elt.first];
            }
        }


        auto deltaDiff = Snapshot<Telement, Tother, nb_others>(m_master.id, diffOthers, diffElements, updateOthers);
        std::vector<u_int8_t> msg = static_cast<std::vector<u_int8_t>>(deltaDiff);

        // server.send(clientId, message);
    }

    std::unordered_map<int, ClientHistory<Telement, Tother, nb_others>> m_clientHistory;
    Snapshot<Telement, Tother, nb_others> m_master;
};
