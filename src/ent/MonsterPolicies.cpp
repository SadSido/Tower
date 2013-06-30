// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: some predefined policies for misc monsters' behavior

#include "MonsterPolicies.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include <assert.h>

//************************************************************************************************************************

// monster does not actually move:

void StandStillPolicy::onStarted(MonsterEntity * owner, const LevelCtx &ctx)
{ owner->enterMoveState(CL_Pointf(), CL_Pointf()); }

void StandStillPolicy::onDetected(MonsterEntity * owner, const LevelCtx &ctx)
{ owner->setFacing(owner->getCenter().x < ctx.player.getCenter().x); }

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

void WalkingPolicy::onDetected(MonsterEntity * owner, const LevelCtx &ctx)
{
	const float speed = owner->getSpeed();
	const bool facing = owner->getCenter().x < ctx.player.getCenter().x;
	const auto newvel = CL_Pointf( facing ? speed : -speed, 0.0f);

	owner->enterMoveState(newvel, CL_Pointf());
}

// monster can fly:

void FlyingPolicy::onStarted(MonsterEntity * owner, const LevelCtx &ctx)
{ 
	m_vel = getRandomVec(Range(360.0f), Range(owner->getSpeed(), owner->getSpeed()));
	owner->enterMoveState(m_vel, CL_Pointf()); 
}

void FlyingPolicy::onCollided(MonsterEntity * owner, const LevelCtx &ctx, const TileTest &test)
{
	m_vel.y *= (test.type == th_Horizontal) ? -1.0f : +1.0f;
	m_vel.x *= (test.type == th_Vertical)   ? -1.0f : +1.0f;

	owner->enterWaitState(); 
}

void FlyingPolicy::onReached(MonsterEntity * owner, const LevelCtx &ctx)
{
	m_vel = getRandomVec(Range(360.0f), Range(owner->getSpeed(), owner->getSpeed()));
	owner->enterWaitState(); 
}

void FlyingPolicy::onWaited(MonsterEntity * owner, const LevelCtx &ctx)
{
	owner->enterMoveState(m_vel, CL_Pointf());
}

void FlyingPolicy::onDetected(MonsterEntity * owner, const LevelCtx &ctx)
{
	auto toPlay = CL_Pointf(ctx.player.getCenter() - owner->getCenter());
	auto newvel = toPlay.normalize() * owner->getSpeed();

	owner->enterMoveState(newvel, CL_Pointf());
}

//************************************************************************************************************************

// monster can only touch player

bool UnarmedPolicy::onTouched  (MonsterEntity * owner, const LevelCtx &ctx)
{ ctx.player.applyDamage(owner->getDamage()); return true; }

// monster has a melee weapon to hit:

bool MeleePolicy::onInRange (MonsterEntity * owner, const LevelCtx &ctx)
{ owner->enterStrikeState(); return true; }

bool MeleePolicy::onTouched (MonsterEntity * owner, const LevelCtx &ctx)
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
