// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entity.h"

//************************************************************************************************************************

Entity::Entity(CL_Pointf pos, CL_Sizef size)
: m_rect(pos, size)
{}

Entity::~Entity()
{}

//************************************************************************************************************************
