/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGame
*/

#pragma once
#include "ECS/Mediator.hpp"
#include "ECS/Managers/System/Systems.hpp"

class RTypeGame
{
public:
    RTypeGame() = default;
    ~RTypeGame() = default;

    void initSystemSignature(const std::string &componentName)
    {
        Signature signature;
        if (componentName == "MotionSystem")
        {
            signature.set(mediator->GetComponentType<Player>());
            signature.set(mediator->GetComponentType<Bullet>());
            signature.set(mediator->GetComponentType<Monster>());
            mediator->SetSystemSignature<MotionSystem>(signature);
            return;
        }
        if (componentName == "InputsPlayer")
        {
            signature.set(mediator->GetComponentType<Player>());
            mediator->SetSystemSignature<InputsPlayer>(signature);
            return;
        }

        if (componentName == "Collision")
        {
            signature.set(mediator->GetComponentType<BoundingBox>());
            mediator->SetSystemSignature<CollisionSystem>(signature);
            return;
        }
    }

    void initGameRules(void)
    {
        mediator = std::make_shared<Mediator>();
        mediator->Init();
        mediator->RegisterComponent<Player>();
        mediator->RegisterComponent<Bullet>();
        mediator->RegisterComponent<Wall>();
        mediator->RegisterComponent<Monster>();
        mediator->RegisterComponent<Transform>();
        mediator->RegisterComponent<Position>();
        mediator->RegisterComponent<BoundingBox>();

        motion = mediator->RegisterSystem<MotionSystem>();
        inputsSystem = mediator->RegisterSystem<InputsPlayer>();
        collision = mediator->RegisterSystem<CollisionSystem>();

        initSystemSignature("MotionSystem");
        initSystemSignature("InputsPlayer");
        initSystemSignature("Collision");
    }

    std::shared_ptr<Mediator> getMediator(void)
    {
        return mediator;
    }

    std::vector<Event> getEvents(void)
    {
        return this->events;
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
    void setEvents(std::vector<Event> newEvents)
    {
        this->events = newEvents;
    }

protected:
private:
    std::shared_ptr<Mediator> mediator;
    std::vector<Event> events;
    std::shared_ptr<MotionSystem> motion;
    std::shared_ptr<InputsPlayer> inputsSystem;
    std::shared_ptr<CollisionSystem> collision;
};
