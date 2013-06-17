// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some predefined policies for misc monsters' behavior

#ifndef _MonsterPolicies_h_
#define _MonsterPolicies_h_

#include "MonsterEntity.h"

//************************************************************************************************************************

// movement policies:

struct NoMovingPolicy : MovingPolicy
{
	virtual void onStarted  (MonsterEntity * owner, const LevelCtx &ctx) override {};
	virtual void onCollided (MonsterEntity * owner, const LevelCtx &ctx) override {};
	virtual void onReached  (MonsterEntity * owner, const LevelCtx &ctx) override {};
};

//************************************************************************************************************************

// attack policies:

struct NoAttackPolicy : AttackPolicy
{
};

//************************************************************************************************************************

#endif
