// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#include "Player.h"
#include "LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

namespace 
{

typedef bool (*LogicFunc)(bool, bool);

bool anyPoint(bool one, bool two)
{ return one || two; }

bool bothPoints(bool one, bool two)
{ return one && two; }

// some tilemap position checkers:

template <TileChecker checker, LogicFunc logic, bool under>
bool testTile(const Tilemap::Ref map, CL_Rectf rect)
{
	const float dy = (under) ? 0.1f : 0.0f;
	const bool one = checker(map->getTile(rect.get_bottom_left()  + CL_Pointf(0.0f, dy)));
	const bool two = checker(map->getTile(rect.get_bottom_right() + CL_Pointf(0.0f, dy)));
	return logic(one, two);
}

bool standOnStairs(const Tilemap::Ref map, CL_Rectf rect)
{ return testTile<anyStairs, bothPoints, false>(map, rect); }

bool standOnGround(const Tilemap::Ref map, CL_Rectf rect)
{ return testTile<anyBlocking, anyPoint, true>(map, rect); }

bool standTopStairs(const Tilemap::Ref map, CL_Rectf rect)
{ return testTile<topStairs, bothPoints, true>(map, rect); }

// some predefined velocities and accelerations:

static const auto v_zero  = CL_Pointf();
static const auto v_climb = CL_Pointf(0.0f, 4.0f); 
static const auto v_walk  = CL_Pointf(4.0f, 0.0f);
static const auto v_jump  = CL_Pointf(0.0f, 9.0f);
static const auto v_leap  = CL_Pointf(0.0f, 6.0f);

static const auto a_zero  = CL_Pointf();
static const auto a_jump  = CL_Pointf(6.0f, 0.0f);
static const auto a_free  = CL_Pointf(0.0f, 12.0f);

}

//************************************************************************************************************************

Player::Player(CL_Pointf pos, CL_Sizef size)
: Entity("player", "player"), m_action(NULL), m_facing(1.0f)
{
	setPos(pos);
	setSize(size);
}

bool Player::update(const LevelCtx &ctx, float secs, int msecs)
{
	// get position flags:
	int posFlags = getPosFlags(ctx);

	// dispatch state-based update:
	switch (getSpriteNo())
	{
	case spr_Stand: { update_Stand (ctx, posFlags); break; }
	case spr_Walk:  { update_Walk  (ctx, posFlags); break; }
	case spr_Climb: { update_Climb (ctx, posFlags); break; }
	case spr_Jump:  { update_Jump  (ctx, posFlags); break; }
	}

	// resolve gravity and movement:
	m_vel += m_acc * secs;

	TileChecker check = (getSpriteNo() == spr_Climb) ? isBlocking : anyBlocking;;
	TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, check);

	if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

	m_rect.translate(moveTest.delta);
	m_facing = (m_vel.x) ? (m_vel.x > 0.0f) ? +1.0f : -1.0f : m_facing;

	// select and update sprite:

	CL_Sprite & sprite = getSprite();
	sprite.update();

	return true;
}

bool Player::render(const LevelCtx &ctx)
{
	CL_Sprite & sprite = getSprite();

	CL_Rectf rect = ctx.tilemap->toScreen(m_rect);
	CL_Draw::box(ctx.gc, rect, CL_Colorf(0,255,0));

	if (m_action)
	{
		CL_Rectf rcAction (rect.get_top_left() - CL_Pointf(0,20), CL_Sizef(20,20));
		CL_Draw::fill(ctx.gc, rcAction, CL_Colorf(255,0,0,100));
	}


	auto anchor = (m_facing > 0.0f) ? rect.get_top_left() : rect.get_top_right();

	sprite.set_scale(m_facing, 1.0f);
	sprite.draw(ctx.gc, anchor.x, anchor.y);

	return true;
}

void Player::upload(const LevelCtx &ctx)
{
	m_sprites.resize(spr_Count);

	m_sprites[spr_Stand] = CL_Sprite(ctx.gc, "arteus_stand", &ctx.assets);
	m_sprites[spr_Walk]  = CL_Sprite(ctx.gc, "arteus_walk",  &ctx.assets);
	m_sprites[spr_Climb] = CL_Sprite(ctx.gc, "arteus_climb", &ctx.assets);
	m_sprites[spr_Jump]  = CL_Sprite(ctx.gc, "arteus_jump",  &ctx.assets);
}

// tilemap check helpers:

int Player::getPosFlags(const LevelCtx &ctx)
{
	int result = 0;
	
	if (standOnGround(ctx.tilemap, m_rect))  { result |= pf_OnGround; }
	if (standOnStairs(ctx.tilemap, m_rect))  { result |= pf_OnStairs; }
	if (standTopStairs(ctx.tilemap, m_rect)) { result |= pf_TopStairs; }
	
	return result;
}

// state-based updates:

void Player::enterState(Sprites spr, CL_Pointf vel, CL_Pointf acc)
{
	m_vel = vel;
	m_acc = acc;

	setSpriteNo(spr);
}

void Player::update_Stand(const LevelCtx &ctx, int posFlags)
{
	// up-key:
	if (ctx.keys.get_keycode(CL_KEY_W))
	{
		if (m_action)
		{
			m_action->doNotify(ctx, n_DoAction);
			m_action = NULL;
		} 
		
		else if (posFlags & pf_OnStairs)
		{ return enterState(spr_Climb, -v_climb, a_zero); }
		
		else
		{ return enterState(spr_Jump, -v_jump, a_free); }
	}

	// down-key:
	if (ctx.keys.get_keycode(CL_KEY_S))
	{
		if (posFlags & pf_TopStairs)
		{ return enterState(spr_Climb, v_climb, a_zero); }
	}

	// left-key:
	if (ctx.keys.get_keycode(CL_KEY_A))
	{ return enterState(spr_Walk, -v_walk, a_zero); }

	// right-key:
	if (ctx.keys.get_keycode(CL_KEY_D))
	{ return enterState(spr_Walk, v_walk, a_zero); }
}

void Player::update_Walk(const LevelCtx &ctx, int posFlags)
{
	// up key:
	if (ctx.keys.get_keycode(CL_KEY_W))
	{
		if (m_action)
		{
			m_action->doNotify(ctx, n_DoAction);
			m_action = NULL;
		} 
		else
		{ return enterState(spr_Jump, CL_Pointf(m_vel.x, -v_leap.y), a_free); }
	}
	
	// left key:
	if (ctx.keys.get_keycode(CL_KEY_A))
	{ return enterState(spr_Walk, -v_walk, a_zero); }

	// right-key:
	else if (ctx.keys.get_keycode(CL_KEY_D))
	{ return enterState(spr_Walk, v_walk, a_zero); }
	
	// no direction:
	else
	{ return enterState(spr_Stand, v_zero, a_zero); }
}

void Player::update_Jump(const LevelCtx &ctx, int posFlags)
{
	// SBA-effects:
	if (posFlags & pf_OnGround)
	{ return enterState(spr_Stand, v_zero, a_zero); }

	// left-key:
	if (ctx.keys.get_keycode(CL_KEY_A))
	{ return enterState(spr_Jump, m_vel, CL_Pointf(-a_jump.x, a_free.y)); }

	// right-key:
	else if (ctx.keys.get_keycode(CL_KEY_D))
	{ return enterState(spr_Jump, m_vel, CL_Pointf(a_jump.x, a_free.y)); }

	// no direction:
	else 
	{ return enterState(spr_Jump, m_vel, a_free); }
}

void Player::update_Climb(const LevelCtx &ctx, int posFlags)
{
	// SPA-effect:
	if (m_vel.y < 0 && !posFlags)
	{
		setSpriteNo(spr_Jump);
	}
	if (m_vel.y > 0 && posFlags & pf_OnGround && !(posFlags & pf_TopStairs))
	{
		setSpriteNo(spr_Stand);
	}

	// up-key:
	if (ctx.keys.get_keycode(CL_KEY_W))
	{
		if (posFlags & pf_OnStairs)
		{
			m_vel.y = -6.0f;
		}
		else if (posFlags & pf_TopStairs)
		{
			m_vel.y = 0.0f;
			setSpriteNo(spr_Stand);
		}
	}

	// down-key:
	if (ctx.keys.get_keycode(CL_KEY_S))
	{
		if (posFlags & pf_OnStairs)
		{
			m_vel.y = +6.0f;
		}
		else if (posFlags & pf_OnGround)
		{
			m_vel.y = 0.0f;
			setSpriteNo(spr_Stand);
		}
	}
}

// action handling:

void Player::setAction(Entity * action)
{ m_action = action; }

bool Player::checkAction(Entity * action)
{ return (m_action == action); }

//************************************************************************************************************************
