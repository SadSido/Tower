// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: shortcuts for creating actual monsters

#include "MonsterTypes.h"
#include "MonsterEntity.h"
#include "MonsterPolicies.h"
#include <assert.h>

//************************************************************************************************************************

Entity::Ref createPlant(const Databags &data, const CL_String &name)
{
	const long stateMask = mask_Emerge | mask_Wait | mask_Vanish;
	return MonsterEntity::create<StandStillPolicy, UnarmedPolicy, AllDamagePolicy>(data, name, stateMask);
}

Entity::Ref createWalker(const Databags &data, const CL_String &name)
{
	const long stateMask = mask_Emerge | mask_Move | mask_Wait | mask_Vanish;
	return MonsterEntity::create<WalkingPolicy, UnarmedPolicy, AllDamagePolicy>(data, name, stateMask);
}

Entity::Ref createGuard(const Databags &data, const CL_String &name)
{
	const long stateMask = mask_Emerge | mask_Wait | mask_Strike | mask_Reload | mask_Vanish;
	return MonsterEntity::create<StandStillPolicy, MeleePolicy, AllDamagePolicy>(data, name, stateMask);
}

Entity::Ref createFootman(const Databags &data, const CL_String &name)
{
	const long stateMask = mask_Emerge | mask_Move | mask_Wait | mask_Strike | mask_Reload | mask_Vanish;
	return MonsterEntity::create<WalkingPolicy, MeleePolicy, AllDamagePolicy>(data, name, stateMask);
}

Entity::Ref createSniper(const Databags &data, const CL_String &name)
{
	const long stateMask = mask_Emerge | mask_Wait | mask_Shoot | mask_Recoil | mask_Reload | mask_Vanish;
	return MonsterEntity::create<StandStillPolicy, ShootingPolicy, AllDamagePolicy>(data, name, stateMask);
}

//************************************************************************************************************************
