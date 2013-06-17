// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for the game monsters

#include "MonsterEntity.h"
#include "MonsterPolicies.h"
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

// c-tors and d-tors:

MonsterEntity::MonsterEntity(const CL_DomNodeList &props)
: m_alive(true), m_speed(0.0f), m_areal(0.0f), m_waittime(0.0f), m_towait(0.0f), m_damage(0.0f)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		// resources prefix:

		if (prop.get_attribute("name") == "prefix") 
		{ m_prefix = prop.get_attribute("value"); }

		// movement parameters:

		if (prop.get_attribute("name") == "areal") 
		{ m_areal = prop.get_attribute_float("value"); }
	
		if (prop.get_attribute("name") == "speed") 
		{ m_speed = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "wait_time") 
		{ m_waittime = prop.get_attribute_float("value"); }

		// combat parameters:

		if (prop.get_attribute("name") == "damage") 
		{ m_damage = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "health") 
		{ m_health = prop.get_attribute_float("value"); }
	}

	// let's have sample for now:
	m_mpolicy = MovingPolicy::Ref(new NoMovingPolicy());
	m_apolicy = AttackPolicy::Ref(new NoAttackPolicy());
}

bool MonsterEntity::update(const LevelCtx &ctx, float secs)
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
		setFacing();

		TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, anyBlocking);

		if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
		if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

		m_rect.translate(moveTest.delta);

		// something like this:
		// if (moveTest.type != th_None)
		// { m_mpolicy->onCollided(this, ctx); }
	}

	// select and update sprite:
	getSprite().update();
	return m_alive;
}

bool MonsterEntity::render(const LevelCtx &ctx)
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

void MonsterEntity::upload(const LevelCtx &ctx)
{
	SpriteVec & sprites = getSprites();
	sprites.resize(state_Count);

	// load sprites:
	for (int stateNo = 0; stateNo < state_Count; ++ stateNo)
	{ sprites[stateNo] = CL_Sprite(ctx.gc, getStateName(stateNo), &ctx.assets); }

	// also, remember initial pos as a base one:
	m_basePos = getCenter();
}

// interface for policies:

void MonsterEntity::enterState(int state)
{
	// ensure valid state:
	assert(state < state_Count);

	// set new sprite:
	if (state != getStateNo())
	{
		setStateNo(state);
		getSprite().restart();
	}
}

// per-state updates:

void MonsterEntity::update_Emerge(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ m_mpolicy->onStarted(this, ctx); }
}

void MonsterEntity::update_Move(const LevelCtx &ctx)
{
	// suffer damage:
	if (!checkDamage(ctx))
	{ return; }

	// apply damage:
	checkPlayer(ctx);

	// out-of-area event:
	if (m_areal && outsideArea())
	{ m_mpolicy->onReached(this, ctx); }
}

void MonsterEntity::update_Wait(const LevelCtx &ctx, float secs)
{
	// suffer damage:
	if (!checkDamage(ctx))
	{ return; }

	// apply damage:
	checkPlayer(ctx);

	// decrement cooldown:
	m_towait = max(0.0f, m_towait - secs);
	
	if (m_towait == 0.0f)
	{ m_mpolicy->onWaited(this, ctx); }
}

void MonsterEntity::update_Vanish(const LevelCtx &ctx)
{
	if (getSprite().is_finished())
	{ m_alive = false; }
}

// damage handling:

bool MonsterEntity::checkDamage(const LevelCtx &ctx)
{
	if (ctx.player.getSwordRect().is_overlapped(m_rect))
	{ doDamage(ctx, 1.0f); }

	// ok, still alive:
	if (m_health > 0.0f)
	{ return true; }

	// or lethal damage:
	m_vel = CL_Pointf();
	m_acc = CL_Pointf();

	enterState(state_Vanish);
	return false;
}

void MonsterEntity::checkPlayer(const LevelCtx &ctx)
{
	if (m_damage && ctx.player.getRect().is_overlapped(m_rect))
	{ ctx.player.doDamage(ctx, m_damage); }
}

// area handling:

bool MonsterEntity::outsideArea()
{
	const auto pos = getCenter();
	return abs(m_basePos.x - pos.x) > m_areal || abs(m_basePos.y - pos.y) > m_areal;
}

//************************************************************************************************************************

