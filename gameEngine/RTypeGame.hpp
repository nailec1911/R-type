/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGame
*/

#pragma once
#include <utility>

#include "ECS/Managers/System/Systems.hpp"
#include "ECS/Mediator.hpp"
#include "Snapshot/SnapshotData.hpp"

namespace gameEngine {
class SystemsFactory
{
   public:
    void initSystems(const std::shared_ptr<Mediator> &mediator)
    {
        motion = mediator->RegisterSystem<MotionSystem>();
        inputsSystem = mediator->RegisterSystem<InputsPlayer>();
        collision = mediator->RegisterSystem<CollisionSystem>();
    }
    std::shared_ptr<MotionSystem> getMotionSystem(void)
    {
        return this->motion;
    }
    std::shared_ptr<InputsPlayer> getInputsSystem(void)
    {
        return this->inputsSystem;
    }
    std::shared_ptr<CollisionSystem> getCollisionSystem(void)
    {
        return this->collision;
    }

   private:
    std::shared_ptr<MotionSystem> motion;
    std::shared_ptr<InputsPlayer> inputsSystem;
    std::shared_ptr<CollisionSystem> collision;
};

class RTypeGame
{
   public:
    RTypeGame() = default;
    ~RTypeGame() = default;
    RTypeGame(const RTypeGame &) = default;
    RTypeGame(RTypeGame &&) = delete;
    RTypeGame &operator=(const RTypeGame &) = default;
    RTypeGame &operator=(RTypeGame &&) = delete;

    enum class SystemType
    {
        MOTION,
        INPUTS,
        COLLISION
    };

    void initSystemSignature(const SystemType &type);
    void initGameRules(void);
    static std::vector<SnapshotData> createSnapshots(
        const std::shared_ptr<Mediator> &mediator);
    void manageTime(void);

    std::shared_ptr<Mediator> getMediator(void)
    {
        return m_mediator;
    }

    std::vector<Event> getEvents(void)
    {
        return this->m_events;
    }

    void setEvents(std::vector<Event> newEvents)
    {
        this->m_events = std::move(newEvents);
    }

   protected:
   private:
    int m_second{};
    time_t m_start_time{};
    std::shared_ptr<Mediator> m_mediator;
    std::vector<Event> m_events;
    gameEngine::SystemsFactory m_systems;
};
}  // namespace gameEngine
