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
	case MonsterEntity::state_Reload:	return "_wait";   // same as wait
	case MonsterEntity::state_Strike:	return "_strike";
	case MonsterEntity::state_Shoot:	return "_shoot";
	case MonsterEntity::state_Recoil:	return "_recoil";
	case MonsterEntity::state_Vanish:	return "_vanish";
	}

	assert(false);
	return CL_String();
};

//************************************************************************************************************************

// c-tors and d-tors:

MonsterEntity::MonsterEntity(const CL_DomNodeList &props, long statesMask)
: m_alive(true), m_statesMask(statesMask), m_speed(0.0f), m_areal(0.0f), m_waittime(0.0f), m_towait(0.0f)
, m_damage(0.0f), m_health(0.0f), m_recover(0.0f), m_detect(0.0f), m_range(0.0f)
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

		if (prop.get_attribute("name") == "range") 
		{ m_range = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "reload") 
		{ m_reload = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "damage") 
		{ m_damage = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "health") 
		{ m_health = prop.get_attribute_float("value"); }

		// shooting parameter (if any):
	}
}

Entity::Ref MonsterEntity::clone()
{
	return Entity::Ref(new MonsterEntity(*this));
}

bool MonsterEntity::update(const LevelCtx &ctx, float secs)
{
	// dispatch state-based update:
	switch (getStateNo())
	{
	case state_Emerge: { update_Emerge (ctx); break; }
	case state_Move:   { update_Move   (ctx); break; }
	case state_Strike: { update_Strike (ctx); break; }
	case state_Wait:   { update_Wait   (ctx, secs); break;  } 
	case state_Reload: { update_Reload (ctx, secs); break;  } 
	case state_Shoot:  { update_Shoot  (ctx); break; }
	case state_Recoil: { update_Recoil (ctx); break; }
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

	// test the sword rect:

	auto swordRect = ctx.tilemap->toScreen(getHitRect());
	CL_Draw::box(ctx.gc, swordRect, CL_Colorf::red);

	return true;
}

void MonsterEntity::upload(const LevelCtx &ctx)
{
	static CL_String s_mapsuffix = "_map";
	static CL_String s_sndsuffix = "_snd";

	SpriteVec & sprites = getSprites();
	sprites.resize(state_Count);

	HitmapVec & hitmaps = getHitmaps();
	hitmaps.resize(state_Count);

	// load sprites:
	for (int stateNo = 0; stateNo < state_Count; ++ stateNo)
	{ 
		if (hasState(stateNo))
		{
			auto name = m_prefix + getStateName(stateNo);
			sprites[stateNo] = CL_Sprite(ctx.gc, name, &ctx.assets);
		}
	}

	// load hitmaps:
	if (hasState(state_Strike))
	{
		auto name = m_prefix + getStateName(state_Strike) + s_mapsuffix; 
		hitmaps[state_Strike] = Hitmap(name, &ctx.assets);
	}

	// load sounds:
	// ...

	// also, remember initial pos as a base one:
	m_basePos = getCenter();
}

// states management:

void MonsterEntity::enterMoveState(CL_Pointf vel, CL_Pointf acc)
{
	setVel(vel); setAcc(acc);
	enterState(state_Move);
}

void MonsterEntity::enterStrikeState()
{ 
	setVel(CL_Pointf(0.0f)); 
	setAcc(CL_Pointf(0.0f));
	enterState(state_Strike);
}

void MonsterEntity::enterShootState()
{ 
	setVel(CL_Pointf(0.0f)); 
	setAcc(CL_Pointf(0.0f));
	enterState(state_Shoot);
}

void MonsterEntity::enterWaitState()
{
	m_towait = m_waittime;
	enterState(state_Wait);
}

void MonsterEntity::enterReloadState()
{
	m_towait = m_reload;
	enterState(state_Reload);
}

void MonsterEntity::enterVanishState()
{ 
	setVel(CL_Pointf(0.0f)); 
	setAcc(CL_Pointf(0.0f));
	enterState(state_Vanish);
}

// damage management:

float MonsterEntity::applyDamage(float ammount)
{
	if (m_recover == 0.0f)
	{ 
		m_health  = max(m_health - ammount, 0.0f);
		m_recover = (m_health) ? s_recoverTime : 0.0f;
	}

	return m_health;
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
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health == 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	// maybe handle "player in range" event:
	if (m_range && detectPlayer(ctx, m_range))
	{ m_apolicy->onInRange(this, ctx); }

	// maybe handle "player detected" event:
	else if (m_detect && detectPlayer(ctx, m_detect))
	{ m_mpolicy->onDetected(this, ctx); }

	// maybe handle "out-of-area" event:
	else if (m_areal && outsideArea(ctx, m_areal))
	{ m_mpolicy->onReached(this, ctx); }
}

void MonsterEntity::update_Wait(const LevelCtx &ctx, float secs)
{
	// suffer damage:
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health == 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	// decrement cooldown:
	m_towait = max(0.0f, m_towait - secs);
	
	// maybe handle "player in range" event:
	if (m_range && detectPlayer(ctx, m_range))
	{ m_apolicy->onInRange(this, ctx); }

	// maybe handle "player detected" event:
	else if (m_detect && detectPlayer(ctx, m_detect))
	{ m_mpolicy->onDetected(this, ctx); }

	// or maybe the wait time expired:
	if (m_towait == 0.0f)
	{ m_mpolicy->onWaited(this, ctx); }
}

void MonsterEntity::update_Reload(const LevelCtx &ctx, float secs)
{
	// suffer damage:
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health == 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	// decrement cooldown:
	m_towait = max(0.0f, m_towait - secs);
	
	// or maybe the wait time expired:
	if (m_towait == 0.0f)
	{ m_mpolicy->onWaited(this, ctx); }
}

void MonsterEntity::update_Strike(const LevelCtx &ctx)
{
	// suffer damage:
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health == 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	if (getSprite().is_finished())
	{ return enterReloadState(); }
}

void MonsterEntity::update_Shoot(const LevelCtx &ctx)
{
	// suffer damage:
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health == 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	if (getSprite().is_finished())
	{
		// spawn missile here and target it:
		return enterState(state_Recoil); 
	}
}

void MonsterEntity::update_Recoil(const LevelCtx &ctx)
{
	// suffer damage:
	if (touchSword(ctx) && m_dpolicy->onDamage(this, ctx) && (m_health == 0.0f))
	{ return; }

	// maybe handle "touch player" event:
	if (m_damage && touchPlayer(ctx) && m_apolicy->onTouched(this, ctx))
	{ return; }

	if (getSprite().is_finished())
	{ return enterReloadState(); }
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

bool MonsterEntity::hasState(int state)
{ return (m_statesMask & (1 << state)) > 0; }

bool MonsterEntity::outsideArea(const LevelCtx &ctx, float distance) const
{
	const auto pos = getCenter();
	return abs(m_basePos.x - pos.x) > distance || abs(m_basePos.y - pos.y) > distance;
}

bool MonsterEntity::detectPlayer(const LevelCtx &ctx, float distance) const
{
	const auto monsterPos = getCenter();
	const auto playerPos  = ctx.player.getCenter();

	return abs(playerPos.x - monsterPos.x) < distance && abs(playerPos.y - monsterPos.y) < distance;
}

bool MonsterEntity::touchPlayer(const LevelCtx &ctx) const
{
	return ctx.player.getRect().is_overlapped(m_rect) ||
		   ctx.player.getRect().is_overlapped(getHitRect());
}

bool MonsterEntity::touchSword(const LevelCtx &ctx) const
{
	return ctx.player.getSwordRect().is_overlapped(m_rect);
}

//************************************************************************************************************************

