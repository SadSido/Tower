// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some predefined policies for misc monsters' behavior

#ifndef _MonsterPolicies_h_
#define _MonsterPolicies_h_

#include "MonsterEntity.h"

//************************************************************************************************************************

// policy with no implementation:

struct NoMovingPolicy : MovingPolicy
{
	virtual void onStarted  (MonsterEntity * owner, const LevelCtx &ctx) override {};
	virtual void onCollided (MonsterEntity * owner, const LevelCtx &ctx) override {};
	virtual void onReached  (MonsterEntity * owner, const LevelCtx &ctx) override {};
	virtual void onWaited   (MonsterEntity * owner, const LevelCtx &ctx) override {};
};

// monster does not actually move:

struct StandStillPolicy : NoMovingPolicy
{
};

//************************************************************************************************************************

// policy with no implementation:

struct NoAttackPolicy : AttackPolicy
{
};

//************************************************************************************************************************

#endif
