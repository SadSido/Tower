// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity(CL_Pointf pos, CL_Sizef size)
: m_rect(pos, size)
{}

Entity::~Entity()
{}

//************************************************************************************************************************
