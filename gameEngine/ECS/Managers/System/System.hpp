/*
** EPITECH PROJECT, 2024
** rts
** File description:
** System
*/

#pragma once
#include <algorithm>
#include <any>
#include <chrono>
#include <vector>

#include "../../../updateDataStruct.hpp"
#include "../../using.hpp"

class ISystem
{
   public:
    virtual ~ISystem() = default;
    virtual void Update(std::any med, DataUpdate &data) = 0;
};

class System : public ISystem
{
   public:
    virtual void Update(std::any med, DataUpdate &data) = 0;
    std::vector<Entity> m_Entities;
};

class MonstersSystem : public System
{
   public:
    virtual void Update(std::any med, DataUpdate &data) = 0;

   protected:
    void addMonsters()
    {
        for (auto entity : m_Entities) {
            if (std::find_if(
                    m_monster.begin(), m_monster.end(),
                    [entity](
                        std::pair<Entity, std::chrono::steady_clock::time_point>
                            pair) { return pair.first == entity; }) ==
                m_monster.end()) {
                m_monster.push_back({entity, {}});
            };
        }
    }

    void eraseMonsters()
    {
        for (auto it = m_monster.begin(); it != m_monster.end();) {
            Entity monsterEntity = it->first;
            if (std::find(
                    m_Entities.begin(), m_Entities.end(), monsterEntity) ==
                m_Entities.end())
                it = m_monster.erase(it);
            else
                ++it;
        }
    }

    std::vector<std::pair<Entity, std::chrono::steady_clock::time_point>>
        m_monster;
};
