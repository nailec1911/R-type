/*
** EPITECH PROJECT, 2024
** rts
** File description:
** RTypeGameClient
*/

#pragma once

#include <vector>
#include "../Renderer/Renderer.hpp"
#include "ARTypeGame.hpp"

namespace gameEngine {
class RTypeGameClient : public gameEngine::ARTypeGame
{
   public:
    RTypeGameClient() = default;
    ~RTypeGameClient() = default;

    void updateRendererState(Renderer &renderer, std::vector<Entity> &entitiesToRemove);
    void updateSystems();
    spritesTypes choosePlayerSprite();

   protected:
   private:
    size_t m_nbPlayers{};
};
}  // namespace gameEngine
