/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGameClient
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "../Renderer/Renderer.hpp"
#include "ARTypeGame.hpp"

namespace gameEngine {
class RTypeGameClient : public gameEngine::ARTypeGame
{
   public:
    RTypeGameClient() = default;
    RTypeGameClient(const RTypeGameClient &) = default;
    RTypeGameClient(RTypeGameClient &&) = delete;
    RTypeGameClient &operator=(const RTypeGameClient &) = default;
    RTypeGameClient &operator=(RTypeGameClient &&) = delete;
    ~RTypeGameClient() = default;

    void initGameRules(void) override;
    void updateRendererState(
        Renderer &renderer, std::vector<Entity> &entitiesToRemove);
    void updateSystems(uint32_t tick, std::vector<Event> &events, int playerEntityId);
    spritesTypes choosePlayerSprite();

    size_t &getNbPlayers()
    {
        return m_nbPlayers;
    }

   protected:
   private:
    size_t m_nbPlayers{};
};
}  // namespace gameEngine
