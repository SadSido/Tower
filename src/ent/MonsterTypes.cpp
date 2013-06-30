// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: shortcuts for creating actual monsters

#include "MonsterTypes.h"
#include "MonsterEntity.h"
#include "MonsterPolicies.h"
#include <assert.h>

//************************************************************************************************************************

Entity::Ref createPlant(const CL_DomNodeList &props)
{
	const long stateMask = mask_Emerge | mask_Move | mask_Vanish;
	return MonsterEntity::create<StandStillPolicy, UnarmedPolicy, AllDamagePolicy>(props, stateMask);
}

//************************************************************************************************************************
