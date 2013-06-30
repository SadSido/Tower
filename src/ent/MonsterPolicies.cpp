// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some predefined policies for misc monsters' behavior

#include "MonsterPolicies.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

// monster does not actually move:

void StandStillPolicy::onStarted(MonsterEntity * owner, const LevelCtx &ctx)
{ 
	owner->enterMoveState(CL_Pointf(), CL_Pointf()); 
}

// monster walks on the ground:

void WalkingPolicy::onStarted(MonsterEntity * owner, const LevelCtx &ctx)
{ 
	m_vel = CL_Pointf(owner->getSpeed(), 0.0f);
	owner->enterMoveState(m_vel, CL_Pointf()); 
}

void WalkingPolicy::onCollided(MonsterEntity * owner, const LevelCtx &ctx, const TileTest &test)
{ owner->enterWaitState(); }

void WalkingPolicy::onReached(MonsterEntity * owner, const LevelCtx &ctx)
{ owner->enterWaitState(); }

void WalkingPolicy::onWaited(MonsterEntity * owner, const LevelCtx &ctx)
{
	m_vel = - m_vel;
	owner->enterMoveState(m_vel, CL_Pointf());
}

//************************************************************************************************************************

// monster can only touch player

bool UnarmedPolicy::onDetected (MonsterEntity * owner, const LevelCtx &ctx)
{ owner->setFacing(owner->getCenter().x < ctx.player.getCenter().x); return false; }

bool UnarmedPolicy::onTouched  (MonsterEntity * owner, const LevelCtx &ctx)
{ ctx.player.applyDamage(owner->getDamage()); return true; }

//************************************************************************************************************************

// monster is damaged equally by any type of attack:

bool AllDamagePolicy::onDamage (MonsterEntity * owner, const LevelCtx &ctx)
{
	if (owner->applyDamage(1.0f) == 0.0f)
	{ owner->enterVanishState(); }

	return true;
}

//************************************************************************************************************************
