// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for the game monsters

#include "MonsterEntity.h"
#include "MonsterPolicies.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include <assert.h>

//************************************************************************************************************************

static const float s_recoverTime = 0.3f;
static const float s_recoverFreq = 1.0f / s_recoverTime;

//************************************************************************************************************************

static CL_Colorf getRecoverColor(float recover)
{
	const float alpha = alphaInterpolateLinear(recover, s_recoverFreq);
	return CL_Colorf(1.0f, 0.0f, 0.0f, alpha);
}

static CL_String getStateName(int state)
{
	switch (state)
	{
	case MonsterEntity::state_Emerge:	return "_emerge";
	case MonsterEntity::state_Move:		return "_move";
	case MonsterEntity::state_Wait:		return "_wait";
	case MonsterEntity::state_Vanish:	return "_vanish";
	}

	assert(false);
	return CL_String();
};

//************************************************************************************************************************

// c-tors and d-tors:

MonsterEntity::MonsterEntity(const CL_DomNodeList &props)
: m_alive(true), m_speed(0.0f), m_areal(0.0f), m_waittime(0.0f), m_towait(0.0f), m_damage(0.0f), m_health(0.0f), m_recover(0.0f), m_detect(0.0f)
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

		if (prop.get_attribute("name") == "detect") 
		{ m_detect = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "damage") 
		{ m_damage = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "health") 
		{ m_health = prop.get_attribute_float("value"); }
	}

	// let's have sample for now:
	m_mpolicy = MovingPolicy::Ref(new NoMovingPolicy());
	m_apolicy = AttackPolicy::Ref(new NoAttackPolicy());
	m_dpolicy = DamagePolicy::Ref(new NoDamagePolicy());
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

	// resolve recovering:
	if (m_recover) 
	{ m_recover = max(m_recover - secs, 0.0f); }

	// resolve movement:
	if (!isRecovering())
	{
		m_vel += m_acc * secs;
		setFacing();

		TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, anyBlocking);
		m_rect.translate(moveTest.delta);

		if (moveTest.type != th_None)
		{ m_mpolicy->onCollided(this, ctx, moveTest); }
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
		sprite.set_color(getRecoverColor(m_recover));
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

// states management:

void MonsterEntity::enterMoveState(CL_Pointf vel, CL_Pointf acc)
{
	setVel(vel); setAcc(acc);
	enterState(state_Move);
}

void MonsterEntity::enterWaitState()
{
	m_towait = m_waittime;
	enterState(state_Wait);
}

// damage management:

void MonsterEntity::applyDamage(float ammount)
{
	if (m_recover == 0.0f)
	{ 
		m_health  = max(m_health - ammount, 0.0f);
		m_recover = (m_health) ? s_recoverTime : 0.0f;
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
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health < 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	// maybe handle "player detected" event:
	if (m_detect && detectPlayer(ctx) && m_apolicy->onDetected(this, ctx))
	{ return; }

	// maybe handle "out-of-area" event:
	if (m_areal && outsideArea(ctx))
	{ m_mpolicy->onReached(this, ctx); }
}

void MonsterEntity::update_Wait(const LevelCtx &ctx, float secs)
{
	// suffer damage:
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health < 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	// maybe handle "player detected" event:
	if (m_detect && detectPlayer(ctx) && m_apolicy->onDetected(this, ctx))
	{ return; }

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

// minor helpers:

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

bool MonsterEntity::outsideArea(const LevelCtx &ctx) const
{
	const auto pos = getCenter();
	return abs(m_basePos.x - pos.x) > m_areal || abs(m_basePos.y - pos.y) > m_areal;
}

bool MonsterEntity::detectPlayer(const LevelCtx &ctx) const
{
	const auto monsterPos = getCenter();
	const auto playerPos  = ctx.player.getCenter();

	return abs(playerPos.x - monsterPos.x) < m_detect || abs(playerPos.y - monsterPos.y) < m_detect;
}

bool MonsterEntity::touchPlayer(const LevelCtx &ctx) const
{
	return ctx.player.getRect().is_overlapped(m_rect);
}

bool MonsterEntity::touchSword(const LevelCtx &ctx) const
{
	return ctx.player.getSwordRect().is_overlapped(m_rect);
}

//************************************************************************************************************************

