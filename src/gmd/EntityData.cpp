// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: custom classes for managing entities data

#include "EntityData.h"
#include <assert.h>

//************************************************************************************************************************

SpriteVec::SpriteVec()
{}

SpriteVec::SpriteVec(const SpriteVec &other)
{
	resize(other.size());

	for (size_t no = 0; no < other.size(); ++ no)
	{ 
		if (!other.at(no).is_null()) 
		{ at(no).clone(other.at(no)); }
	}
}

//************************************************************************************************************************
