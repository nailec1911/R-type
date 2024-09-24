/*
** EPITECH PROJECT, 2024
** rts
** File description:
** IComponentArray
*/

#pragma once
#include "../../using.hpp"

class IComponentArray
{
   public:
    IComponentArray(const IComponentArray &) = default;
    IComponentArray(IComponentArray &&) = delete;
    IComponentArray &operator=(const IComponentArray &) = default;
    IComponentArray &operator=(IComponentArray &&) = delete;
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};
