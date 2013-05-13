// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: patrolling monsters or NPCs

#include "PatrolEntity.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

enum States
{
	state_Emerge,
	state_Move,
	state_Wait,
	state_Vanish,
	state_Count,
};

static CL_String getStateName(int state)
{
	switch (state)
	{
	case state_Emerge:  return "_emerge";
	case state_Move:	return "_move";
	case state_Wait:	return "_wait";
	case state_Vanish:	return "_vanish";
	}

	assert(false);
	return CL_String();
};

//************************************************************************************************************************

BasePatrol::BasePatrol(const CL_DomNodeList &props)
: m_alive(true), m_speed(0.0f), m_distance(0.0f), m_waittime(0.0f), m_towait(0.0f), m_damage(0.0f)
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

		if (prop.get_attribute("name") == "damage") 
		{ m_damage = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "health") 
		{ m_health = prop.get_attribute_float("value"); }
	}
}

bool BasePatrol::update(const LevelCtx &ctx, float secs)
{
	// dispatch state-based update:
	switch (getStateNo())
	{
	case state_Emerge: { update_Emerge (ctx); break; }
	case state_Move:   { update_Move   (ctx); break; }
	case state_Wait:   { update_Wait   (ctx, secs); break;  } 
	case state_Vanish: { update_Vanish (ctx); break; }
	}

	// resolve movement:
	if (!isRecovering())
	{
		m_vel += m_acc * secs;
		m_rect.translate(m_vel * secs);
		setFacing();
	}

	// select and update sprite:
	getSprite().update();
	return m_alive;
}

bool BasePatrol::render(const LevelCtx &ctx)
{
	CL_Sprite & sprite = getSprite();

	CL_Rectf rect = ctx.tilemap->toScreen(m_rect);
	CL_Draw::box(ctx.gc, rect, CL_Colorf(0,255,0));

	auto facing = getFacing();
	auto anchor = (facing > 0.0f) ? rect.get_top_left() : rect.get_top_right();

	sprite.set_scale(facing, 1.0f);
	sprite.set_color(CL_Color::white);
	sprite.draw(ctx.gc, anchor.x, anchor.y);

	if (isRecovering())
	{
		sprite.set_color(getRecoverColor());
		sprite.draw(ctx.gc, anchor.x, anchor.y);
	}

	return true;
}

void BasePatrol::upload(const LevelCtx &ctx)
{
	SpriteVec & sprites = getSprites();
	sprites.resize(state_Count);

	// load sprites:
	for (int stateNo = 0; stateNo < state_Count; ++ stateNo)
	{ sprites[stateNo] = CL_Sprite(ctx.gc, m_prefix + getStateName(stateNo), &ctx.assets); }

	// also, remember initial pos as a base one:
	m_basePos = getCenter();
}

// state-based updates:

void BasePatrol::enterState(int state, CL_Pointf vel)
{
	// ensure valid state:
	assert(state < state_Count);

	// set movement params:
	m_vel = vel;

	// set new sprite:
	if (state != getStateNo())
	{
		setStateNo(state);
		getSprite().restart();
	}
}

void BasePatrol::update_Emerge(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ setNextPos(); enterState(state_Move, m_vel); }
}

void BasePatrol::update_Move(const LevelCtx &ctx)
{
	if (!checkDamage(ctx))
	{ enterState(state_Vanish, CL_Pointf()); }

	// try to hit player:
	checkPlayer(ctx);

	if (reachedPos())
	{ m_towait = m_waittime; enterState(state_Wait, CL_Pointf()); }
}

void BasePatrol::update_Wait(const LevelCtx &ctx, float secs)
{
	if (!checkDamage(ctx))
	{ enterState(state_Vanish, CL_Pointf()); }

	// try to hit player:
	checkPlayer(ctx);

	// decrement cooldown:
	m_towait = max(0.0f, m_towait - secs);
	
	if (m_towait == 0.0f)
	{ setNextPos(); enterState(state_Move, m_vel); }
}

void BasePatrol::update_Vanish(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ m_alive = false; }
}

// damage handling:

bool BasePatrol::checkDamage(const LevelCtx &ctx)
{
	if (ctx.player.getSwordRect().is_overlapped(m_rect))
	{ doDamage(ctx, 1.0f); }

	return (m_health > 0.0f);
}

void BasePatrol::checkPlayer(const LevelCtx &ctx)
{
	if (m_damage && ctx.player.getRect().is_overlapped(m_rect))
	{ ctx.player.doDamage(ctx, m_damage); }
}

//************************************************************************************************************************

FlyingPatrol::FlyingPatrol(const CL_DomNodeList &props)
: BasePatrol(props)
{}

FlyingPatrol::Ref FlyingPatrol::clone()
{
	return FlyingPatrol::Ref(new FlyingPatrol(*this));
}

void FlyingPatrol::setNextPos()
{
	const int newx = rand() % 100 - 50, newy = rand() % 100 - 50;
	const CL_Pointf direct = CL_Pointf(float(newx), float(newy)).normalize();
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

GroundPatrol::GroundPatrol(const CL_DomNodeList &props)
: BasePatrol(props)
{}

GroundPatrol::Ref GroundPatrol::clone()
{
	return GroundPatrol::Ref(new GroundPatrol(*this));
}

void GroundPatrol::setNextPos()
{
	const float direction = (rand() % 200 - 100) / (float)100;
	const CL_Pointf delta = CL_Pointf(direction * m_distance, 0);

	m_nextPos = m_basePos + delta;
	m_vel = CL_Pointf(m_speed, 0.0f) * ((direction > 0.0f) ? +1.0f : -1.0f);
}

bool GroundPatrol::reachedPos()
{
	const CL_Pointf toTarget = getCenter() - m_nextPos;
	return (abs(toTarget.x) + abs(toTarget.y)) < 2.0f;
}

//************************************************************************************************************************

