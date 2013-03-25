// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for a flying passievely patrolling monster

#include "FlyingPatrol.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

FlyingPatrol::FlyingPatrol(const CL_DomNodeList &plist)
: m_alive(true)
{
	
}

Entity::Ref FlyingPatrol::clone()
{
	return Entity::Ref();
}

bool FlyingPatrol::update(const LevelCtx &ctx, float secs)
{
	// dispatch state-based update:
	switch (getSpriteNo())
	{
	case spr_Emerge: { update_Emerge (ctx); break; }
	case spr_Move:   { update_Move   (ctx); break; }
	case spr_Vanish: { update_Vanish (ctx); break; }
	}

	// resolve movement:
	m_vel += m_acc * secs;
	m_rect.translate(m_vel * secs);
	setFacing();

	// select and update sprite:
	getSprite().update();
	return m_alive;
}

bool FlyingPatrol::render(const LevelCtx &ctx)
{
	CL_Sprite & sprite = getSprite();

	CL_Rectf rect = ctx.tilemap->toScreen(m_rect);
	CL_Draw::box(ctx.gc, rect, CL_Colorf(0,255,0));

	auto facing = getFacing();
	auto anchor = (facing > 0.0f) ? rect.get_top_left() : rect.get_top_right();

	sprite.set_scale(facing, 1.0f);
	sprite.draw(ctx.gc, anchor.x, anchor.y);

	return true;
}

// state-based updates:

void FlyingPatrol::enterState(Sprites spr)
{
	if (spr != getSpriteNo())
	{
		setSpriteNo(spr);
		getSprite().restart();
	}
}

void FlyingPatrol::update_Emerge(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ setNextPos(); enterState(spr_Move); }
}

void FlyingPatrol::update_Move(const LevelCtx &ctx)
{
	if (reachedPos())
	{ setNextPos(); }
}

void FlyingPatrol::update_Vanish(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ m_alive = false; }
}

// helpers:

void FlyingPatrol::setNextPos()
{
	const CL_Pointf direct = CL_Pointf(rand(), rand()).normalize();
	const CL_Pointf delta  = direct * m_distance;

	m_nextPos = m_basePos + delta;
	m_vel = (getCenter() - m_nextPos).normalize() * m_maxvel;
}

bool FlyingPatrol::reachedPos()
{
	const CL_Pointf toTarget = getCenter() - m_nextPos;
	return (abs(toTarget.x) + abs(toTarget.y)) < 1.0f;
}


//************************************************************************************************************************
