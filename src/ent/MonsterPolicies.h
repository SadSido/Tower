// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some predefined policies for misc monsters' behavior

#ifndef _MonsterPolicies_h_
#define _MonsterPolicies_h_

#include "MonsterEntity.h"

//************************************************************************************************************************

// policy with no implementation:

struct NoMovingPolicy : MovingPolicy
{
	virtual void onStarted  (MonsterEntity * owner, const LevelCtx &ctx) override {}
	virtual void onReached  (MonsterEntity * owner, const LevelCtx &ctx) override {}
	virtual void onWaited   (MonsterEntity * owner, const LevelCtx &ctx) override {}
	virtual void onDetected (MonsterEntity * owner, const LevelCtx &ctx) override {}
	virtual void onCollided (MonsterEntity * owner, const LevelCtx &ctx, const TileTest &test) override {}
};

// monster does not actually move:

struct StandStillPolicy : NoMovingPolicy
{
	virtual void onStarted  (MonsterEntity * owner, const LevelCtx &ctx) override;
	virtual void onDetected (MonsterEntity * owner, const LevelCtx &ctx) override;
};

// monster walks on the ground:

struct WalkingPolicy : NoMovingPolicy
{
	CL_Pointf m_vel;

	virtual void onStarted  (MonsterEntity * owner, const LevelCtx &ctx) override;
	virtual void onReached  (MonsterEntity * owner, const LevelCtx &ctx) override;
	virtual void onWaited   (MonsterEntity * owner, const LevelCtx &ctx) override;
	virtual void onDetected (MonsterEntity * owner, const LevelCtx &ctx) override;
	virtual void onCollided (MonsterEntity * owner, const LevelCtx &ctx, const TileTest &test) override;
};

//************************************************************************************************************************

// policy with no implementation:

struct NoAttackPolicy : AttackPolicy
{
	virtual bool onTouched  (MonsterEntity * owner, const LevelCtx &ctx) override { return false; }
};

// monster can only touch player:

struct UnarmedPolicy : NoAttackPolicy
{
	virtual bool onTouched  (MonsterEntity * owner, const LevelCtx &ctx) override;
};

//************************************************************************************************************************

struct NoDamagePolicy : DamagePolicy
{
	virtual bool onDamage (MonsterEntity * owner, const LevelCtx &ctx) override { return false; }
};

// standard damage policy:

struct AllDamagePolicy : NoDamagePolicy
{
	virtual bool onDamage (MonsterEntity * owner, const LevelCtx &ctx) override;
};

//************************************************************************************************************************

#endif
