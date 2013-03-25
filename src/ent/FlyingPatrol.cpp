// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for a flying passievely patrolling monster

#include "FlyingPatrol.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

FlyingPatrol::FlyingPatrol(CL_Pointf pos, CL_Sizef size)
{
	setPos(pos);
	setSize(size);
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

	/*
	// resolve movement:
	m_vel += m_acc * secs;

	TileChecker check = (getSpriteNo() == spr_Climb) ? isBlocking : anyBlocking;;
	TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, check);

	if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

	m_rect.translate(moveTest.delta);
	m_facing = (m_vel.x) ? (m_vel.x > 0.0f) ? +1.0f : -1.0f : m_facing;
	*/

	// select and update sprite:
	getSprite().update();

	return true;
}

bool FlyingPatrol::render(const LevelCtx &ctx)
{
	CL_Sprite & sprite = getSprite();

	CL_Rectf rect = ctx.tilemap->toScreen(m_rect);
	CL_Draw::box(ctx.gc, rect, CL_Colorf(0,255,0));

	auto anchor = (m_facing > 0.0f) ? rect.get_top_left() : rect.get_top_right();

	sprite.set_scale(m_facing, 1.0f);
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
}

void FlyingPatrol::update_Move(const LevelCtx &ctx)
{
}

void FlyingPatrol::update_Vanish(const LevelCtx &ctx)
{
}

//************************************************************************************************************************
