// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for a flying passievely patrolling monster

#include "FlyingPatrol.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

FlyingPatrol::FlyingPatrol(const CL_DomNodeList &props)
: m_alive(true), m_speed(0.0f), m_distance(0.0f), m_waittime(0.0f), m_towait(0.0f)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "distance") 
		{ m_distance = prop.get_attribute_float("value"); }
	
		if (prop.get_attribute("name") == "speed") 
		{ m_speed = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "wait_time") 
		{ m_waittime = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "prefix") 
		{ m_prefix = prop.get_attribute("value"); }
	}
}

Entity::Ref FlyingPatrol::clone()
{
	return Entity::Ref(new FlyingPatrol(*this));
}

bool FlyingPatrol::update(const LevelCtx &ctx, float secs)
{
	// dispatch state-based update:
	switch (getSpriteNo())
	{
	case spr_Emerge: { update_Emerge (ctx); break; }
	case spr_Move:   { update_Move   (ctx); break; }
	case spr_Wait:   { update_Wait   (ctx, secs); break;  } 
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

void FlyingPatrol::upload(const LevelCtx &ctx)
{
	SpriteVec & sprites = getSprites();
	sprites.resize(spr_Count);

	sprites[spr_Emerge] = CL_Sprite(ctx.gc, m_prefix + "_emerge", &ctx.assets);
	sprites[spr_Wait]   = CL_Sprite(ctx.gc, m_prefix + "_wait",   &ctx.assets);
	sprites[spr_Move]   = CL_Sprite(ctx.gc, m_prefix + "_move",   &ctx.assets);
	sprites[spr_Vanish] = CL_Sprite(ctx.gc, m_prefix + "_vanish", &ctx.assets);

	// also, remember initial pos as a base one:
	m_basePos = getCenter();
}

// state-based updates:

void FlyingPatrol::enterState(Sprites spr, CL_Pointf vel)
{
	// set movement params:
	m_vel = vel;

	// set new sprite:
	if (spr != getSpriteNo())
	{
		setSpriteNo(spr);
		getSprite().restart();
	}
}

void FlyingPatrol::update_Emerge(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ setNextPos(); enterState(spr_Move, m_vel); }
}

void FlyingPatrol::update_Move(const LevelCtx &ctx)
{
	if (reachedPos())
	{ m_towait = m_waittime; enterState(spr_Wait, CL_Pointf()); }
}

void FlyingPatrol::update_Wait(const LevelCtx &ctx, float secs)
{
	// decrement cooldown:
	m_towait = max(0.0f, m_towait - secs);
	
	if (m_towait == 0.0f)
	{ setNextPos(); enterState(spr_Move, m_vel); }
}

void FlyingPatrol::update_Vanish(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ m_alive = false; }
}

// helpers:

void FlyingPatrol::setNextPos()
{
	const CL_Pointf direct = CL_Pointf(rand() % 100 - 50, rand() % 100 - 50).normalize();
	const CL_Pointf delta  = direct * m_distance;

	m_nextPos = m_basePos + delta;
	m_vel = (m_nextPos - getCenter()).normalize() * m_speed;
}

bool FlyingPatrol::reachedPos()
{
	const CL_Pointf toTarget = getCenter() - m_nextPos;
	return (abs(toTarget.x) + abs(toTarget.y)) < 2.0f;
}


//************************************************************************************************************************
