/*
** EPITECH PROJECT, 2024
** rts
** File description:
** ARTypeGame
*/

#pragma once
#include "../ECS/Mediator.hpp"

namespace gameEngine {
class SystemsFactory
{
   public:
    template <typename T>
    void addSystem(const std::shared_ptr<Mediator> &mediator)
    {
        std::shared_ptr<ISystem> system = mediator->RegisterSystem<T>();
        m_systems.push_back(system);
    }

    std::vector<std::shared_ptr<ISystem>> getSystems() const
    {
        return m_systems;
    }

   private:
    std::vector<std::shared_ptr<ISystem>> m_systems;
};

class ARTypeGame
{
   public:
    ARTypeGame();
    ~ARTypeGame();

    enum class SystemType
    {
        MOTION,
        INPUTS,
        COLLISION,
        BULLETDESTRUCTION,
        SHOOTERMONSTER,
        FLYINGMONSTER,
        PLAYERBORDER,
        DESTROYENTITIES
    };
    void initGameRules(void);
    void initSystemSignature(const SystemType &type);
    void createEntity(
        const EntityName &name, std::pair<float, float> pos, int id);

    void createWall(std::pair<float, float> &pos, int id);
    void createShooterMonster(std::pair<float, float> &pos, int id);
    void createFlyingMonster(std::pair<float, float> &pos, int id);
    void createPlayer(std::pair<float, float> &pos, int id);
    void createMBullet(std::pair<float, float> &pos, int id);
    void createPBullet(std::pair<float, float> &pos, int id);

    SystemsFactory getSystems(void)
    {
        return m_systems;
    }

    std::shared_ptr<Mediator> getMediator(void)
    {
        return m_mediator;
    }

   protected:
    std::shared_ptr<Mediator> m_mediator;
    gameEngine::SystemsFactory m_systems;

   private:
};
}  // namespace gameEngine