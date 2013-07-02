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

Entity::Ref createWalker(const CL_DomNodeList &props)
{
	const long stateMask = mask_Emerge | mask_Move | mask_Wait | mask_Vanish;
	return MonsterEntity::create<WalkingPolicy, UnarmedPolicy, AllDamagePolicy>(props, stateMask);
}

Entity::Ref createGuard(const CL_DomNodeList &props)
{
	const long stateMask = mask_Emerge | mask_Move | mask_Wait | mask_Strike | mask_Vanish;
	return MonsterEntity::create<StandStillPolicy, MeleePolicy, AllDamagePolicy>(props, stateMask);
}

Entity::Ref createFootman(const CL_DomNodeList &props)
{
	const long stateMask = mask_Emerge | mask_Move | mask_Wait | mask_Strike | mask_Vanish;
	return MonsterEntity::create<WalkingPolicy, MeleePolicy, AllDamagePolicy>(props, stateMask);
}

//************************************************************************************************************************
