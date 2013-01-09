// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for every entity in the game

#include "Entities.h"

//************************************************************************************************************************

// c-tor and d-tor:

Entity::Entity(CL_String type, CL_String name)
: m_type(type), m_name(name)
{}

Entity::~Entity()
{}

//************************************************************************************************************************
