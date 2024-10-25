/*
** EPITECH PROJECT, 2024
** rts
** File description:
** ARTypeGame
*/

#include "ARTypeGame.hpp"

#include "../ECS/Managers/Component/StructComponent.hpp"
#include "../ECS/Managers/System/Systems.hpp"

gameEngine::ARTypeGame::ARTypeGame() {}

gameEngine::ARTypeGame::~ARTypeGame() {}

void gameEngine::ARTypeGame::initSystemSignature(const SystemType &type)
{
    Signature signature;
    if (type == SystemType::MOTION) {
        signature.set(m_mediator->GetComponentType<Player>());
        signature.set(m_mediator->GetComponentType<BulletPlayer>());
        signature.set(m_mediator->GetComponentType<BulletMonster>());
        signature.set(m_mediator->GetComponentType<BulletPlayer>());
        signature.set(m_mediator->GetComponentType<BulletMonster>());
        signature.set(m_mediator->GetComponentType<Monster>());
        signature.set(m_mediator->GetComponentType<Wall>());
        m_mediator->SetSystemSignature<MotionSystem>(signature);
        return;
    }
    if (type == SystemType::INPUTS) {
        signature.set(m_mediator->GetComponentType<Player>());
        m_mediator->SetSystemSignature<InputsPlayer>(signature);
        return;
    }

    if (type == SystemType::INPUTS_CLIENT) {
        signature.set(m_mediator->GetComponentType<Player>());
        m_mediator->SetSystemSignature<InputsPlayerClient>(signature);
        return;
    }

    if (type == SystemType::COLLISION) {
        signature.set(m_mediator->GetComponentType<BoundingBox>());
        m_mediator->SetSystemSignature<CollisionSystem>(signature);
        return;
    }
    if (type == SystemType::BULLETDESTRUCTION) {
        signature.set(m_mediator->GetComponentType<BulletPlayer>());
        m_mediator->SetSystemSignature<DestroyBullets>(signature);
        return;
    }
    if (type == SystemType::SHOOTERMONSTER) {
        signature.set(m_mediator->GetComponentType<ShooterMonster>());
        m_mediator->SetSystemSignature<ShootingMonsterSystem>(signature);
        return;
    }
    if (type == SystemType::FLYINGMONSTER) {
        signature.set(m_mediator->GetComponentType<FlyingMonster>());
        m_mediator->SetSystemSignature<FlyingMonsterSystem>(signature);
        return;
    }
    if (type == SystemType::PLAYERBORDER) {
        signature.set(m_mediator->GetComponentType<Player>());
        m_mediator->SetSystemSignature<PlayerBorderSystem>(signature);
        return;
    }
    if (type == SystemType::DESTROYENTITIES) {
        signature.set(m_mediator->GetComponentType<Wall>());
        signature.set(m_mediator->GetComponentType<BulletMonster>());
        signature.set(m_mediator->GetComponentType<Monster>());
        m_mediator->SetSystemSignature<DestroyEntities>(signature);
        return;
    }
}

void gameEngine::ARTypeGame::createEntity(
    const EntityName &name, std::pair<float, float> pos, int id, uint32_t tick)
{
    if (name == WALL) {
        createWall(pos, id, tick);
        return;
    }
    if (name == SHOOTER_MONSTER) {
        createShooterMonster(pos, id, tick);
        return;
    }
    if (name == FLYING_MONSTER) {
        createFlyingMonster(pos, id, tick);
        return;
    }
    if (name == PLAYER) {
        createPlayer(pos, id, tick);
        return;
    }
    if (name == M_BULLET) {
        createMBullet(pos, id, tick);
        return;
    }
    if (name == P_BULLET) {
        createPBullet(pos, id, tick);
        return;
    }
}

void gameEngine::ARTypeGame::createWall(
    std::pair<float, float> &pos, int id, uint32_t tick)
{
    Entity wall =
        id >= 0 ? m_mediator->CreateEntityById(id) : m_mediator->CreateEntity();
    m_mediator->AddComponent(wall, Chrono{tick});
    m_mediator->AddComponent<Wall>(wall, {});
    m_mediator->AddComponent<Position>(
        wall, {pos.first, pos.second, pos.first, pos.second});
    m_mediator->AddComponent<Transform>(wall, {-100, 0});
    m_mediator->AddComponent<BoundingBox>(wall, {138, 138});
}

void gameEngine::ARTypeGame::createShooterMonster(
    std::pair<float, float> &pos, int id, uint32_t tick)
{
    Entity monster =
        id >= 0 ? m_mediator->CreateEntityById(id) : m_mediator->CreateEntity();
    m_mediator->AddComponent(monster, Chrono{tick});
    m_mediator->AddComponent<Monster>(monster, {});
    m_mediator->AddComponent<ShooterMonster>(monster, {});
    m_mediator->AddComponent<Position>(
        monster, {pos.first, pos.second, pos.first, pos.second});
    m_mediator->AddComponent<Transform>(monster, {-100, 0});
    m_mediator->AddComponent<BoundingBox>(monster, {93, 93});
}

void gameEngine::ARTypeGame::createFlyingMonster(
    std::pair<float, float> &pos, int id, uint32_t tick)
{
    Entity monster =
        id >= 0 ? m_mediator->CreateEntityById(id) : m_mediator->CreateEntity();
    m_mediator->AddComponent(monster, Chrono{tick});
    m_mediator->AddComponent<Monster>(monster, {});
    m_mediator->AddComponent<FlyingMonster>(monster, {});
    m_mediator->AddComponent<Position>(
        monster, {pos.first, pos.second, pos.first, pos.second});
    m_mediator->AddComponent<Transform>(monster, {-200, 150});
    m_mediator->AddComponent<BoundingBox>(monster, {60, 69});
}

void gameEngine::ARTypeGame::createPlayer(
    std::pair<float, float> &pos, int id, uint32_t tick)
{
    Entity newPlayer =
        id >= 0 ? m_mediator->CreateEntityById(id) : m_mediator->CreateEntity();
    m_mediator->AddComponent(newPlayer, Chrono{tick});
    m_mediator->AddComponent<Player>(newPlayer, {});
    m_mediator->AddComponent<Transform>(
        newPlayer, Transform{.velX = 0, .velY = 0});
    m_mediator->AddComponent<Position>(
        newPlayer, Position{
                       .x = pos.first,
                       .y = pos.second,
                       .initX = pos.first,
                       .initY = pos.second});
    m_mediator->AddComponent<BoundingBox>(newPlayer, BoundingBox{93, 33});
}

void gameEngine::ARTypeGame::createMBullet(
    std::pair<float, float> &pos, int id, uint32_t tick)
{
    Entity bullet =
        id >= 0 ? m_mediator->CreateEntityById(id) : m_mediator->CreateEntity();
    m_mediator->AddComponent(bullet, Chrono{tick});
    m_mediator->AddComponent(bullet, BulletMonster{});
    m_mediator->AddComponent(bullet, Transform{.velX = -800, .velY = 0});
    m_mediator->AddComponent(bullet, BoundingBox{10, 10});
    m_mediator->AddComponent<Position>(
        bullet, {pos.first, pos.second, pos.first, pos.second});
}

void gameEngine::ARTypeGame::createPBullet(
    std::pair<float, float> &pos, int id, uint32_t tick)
{
    Entity bullet =
        id >= 0 ? m_mediator->CreateEntityById(id) : m_mediator->CreateEntity();
    m_mediator->AddComponent(bullet, Chrono{tick});
    m_mediator->AddComponent(bullet, BulletPlayer{});
    m_mediator->AddComponent(bullet, BoundingBox{10, 10});
    m_mediator->AddComponent(bullet, Transform{.velX = 800, .velY = 0});
    m_mediator->AddComponent(
        bullet, Position{pos.first, pos.second, pos.first, pos.second});
}
