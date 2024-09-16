/*
** EPITECH PROJECT, 2024
** rts
** File description:
** IComponentArray
*/

#pragma once
#include "using.hpp"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};
