// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for the game monsters

#include "MonsterEntity.h"
#include "MonsterPolicies.h"
#include "MissileEntity.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include "../util/XmlUtils.h"
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

MonsterEntity::MonsterEntity(const Databags &data, const CL_String &name, long statesMask)
: m_alive(true), m_statesMask(statesMask), m_towait(0.0f), m_recover(0.0f)
{
	auto bag = data.find(name)->second;
	
	m_prefix	= bag->get<CL_String>("sprite");
	m_areal		= bag->get<float>("areal");
	m_speed		= bag->get<float>("speed");
	m_waittime	= bag->get<float>("wait_time");
	m_detect	= bag->get<float>("detect");
	m_range		= bag->get<float>("range");
	m_reload	= bag->get<float>("reload");
	m_damage	= bag->get<float>("damage");
	m_health	= bag->get<float>("health");
	m_barrel	= bag->get<CL_Pointf>("shoot_pos");
	
	if (hasState(state_Shoot))
	{
		auto name = bag->get<CL_String>("shoot_name");
		m_missile = Entity::Ref(new MissileEntity(data, name));
	}
}

Entity::Ref MonsterEntity::clone()
{
	return Entity::Ref(new MonsterEntity(*this));
}

// virtual interface:

bool MonsterEntity::update(const LevelCtx &ctx, float secs)
{
	// upload missile once:
	if (m_missile)
	{ m_missile->doUpload(ctx); }

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

	// shoot and play recoil animation:
	if (getSprite().is_finished())
	{
		Entity::Ref missile = m_missile->clone();

		CL_Pointf anchor = (getFacing() > 0.0f) ? getRect().get_top_left() : getRect().get_top_right();
		CL_Pointf barrel = CL_Pointf(m_barrel.x * (getFacing() > 0.0f) ? +1.0f : -1.0f, m_barrel.y);

		missile->setPos(anchor + barrel);
		missile->doNotify(ctx, n_LockTarget);

		ctx.entities->push_back(missile);
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

