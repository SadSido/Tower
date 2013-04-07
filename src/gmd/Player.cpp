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
	const float dy = (under) ? 0.01f : 0.0f;
	const bool one = checker(map->getTile(rect.get_bottom_left()  + CL_Pointf(0.0f, dy)));
	const bool two = checker(map->getTile(rect.get_bottom_right() + CL_Pointf(0.0f, dy)));
	return logic(one, two);
}

bool standOnStairs(const Tilemap::Ref map, CL_Rectf rect)
{ return testTile<anyStairs, bothPoints, false>(map, rect); }

bool standOnGround(const Tilemap::Ref map, CL_Rectf rect)
{ return testTile<anyBlocking, anyPoint, true>(map, rect) &&
        !testTile<anyBlocking, anyPoint, false>(map, rect); }

bool standTopStairs(const Tilemap::Ref map, CL_Rectf rect)
{ return testTile<topStairs, bothPoints, true>(map, rect); }

// some predefined velocities and accelerations:

static const auto v_zero  = CL_Pointf();
static const auto v_climb = CL_Pointf(0.0f,  4.0f); 
static const auto v_walk  = CL_Pointf(4.0f,  0.0f);
static const auto v_jump  = CL_Pointf(0.0f, 12.0f);
static const auto v_leap  = CL_Pointf(0.0f,  8.0f);
static const auto v_pier  = CL_Pointf(0.0f,  3.0f);

static const auto a_zero  = CL_Pointf();
static const auto a_jump  = CL_Pointf(6.0f,  0.0f);
static const auto a_free  = CL_Pointf(0.0f, 20.0f);

// enumerations and states:

enum PosFlags
{
	pf_OnGround  = 1 << 0,
	pf_OnStairs  = 1 << 1,
	pf_TopStairs = 1 << 2,
};

enum States
{
	state_Stand,
	state_Walk,
	state_Climb,
	state_Jump,
	state_Shield,
	state_Pierce,
	state_Slash,
	state_Strike,
	state_Count
};

static CL_String getStateName(int state)
{
	switch (state)
	{
	case state_Stand:	return "_stand";
	case state_Walk:	return "_walk";
	case state_Climb:	return "_climb";
	case state_Jump:	return "_jump";
	case state_Shield:	return "_shield";
	case state_Pierce:	return "_pierce";
	case state_Slash:	return "_slash";
	case state_Strike:	return "_strike";
	}

	assert(false);
	return CL_String();
}

}

//************************************************************************************************************************

Player::Player(CL_Pointf pos, CL_Sizef size)
: m_action(NULL)
{
	setPos(pos);
	setSize(size);
}

bool Player::update(const LevelCtx &ctx, float secs)
{
	// get position flags:
	int posFlags = getPosFlags(ctx);

	// dispatch state-based update:
	switch (getStateNo())
	{
	case state_Stand:  { update_Stand  (ctx, posFlags); break; }
	case state_Walk:   { update_Walk   (ctx, posFlags); break; }
	case state_Climb:  { update_Climb  (ctx, posFlags); break; }
	case state_Jump:   { update_Jump   (ctx, posFlags); break; }
	case state_Shield: { update_Shield (ctx, posFlags); break; }
	case state_Pierce: { update_Pierce (ctx, posFlags); break; }
	case state_Slash:  { update_Slash  (ctx, posFlags); break; }
	case state_Strike: { update_Strike (ctx, posFlags); break; }
	}

	// resolve movement:
	m_vel += m_acc * secs;
	setFacing();

	TileChecker check = (getStateNo() == state_Climb) ? isBlocking : anyBlocking;;
	TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, check);

	if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

	m_rect.translate(moveTest.delta);

	// update sprite:
	getSprite().update();
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


	auto facing = getFacing();
	auto anchor = (facing > 0.0f) ? rect.get_top_left() : rect.get_top_right();

	sprite.set_scale(facing, 1.0f);
	sprite.draw(ctx.gc, anchor.x, anchor.y);

	return true;
}

void Player::upload(const LevelCtx &ctx)
{
	static CL_String s_prefix = "arteus";

	SpriteVec & sprites = getSprites();
	sprites.resize(state_Count);

	// load sprites:
	for (int stateNo = 0; stateNo < state_Count; ++ stateNo)
	{ sprites[stateNo] = CL_Sprite(ctx.gc, s_prefix + getStateName(stateNo), &ctx.assets); }
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

void Player::enterAction(const LevelCtx &ctx)
{
	m_action->doNotify(ctx, n_DoAction);
	m_action = NULL;
}

void Player::enterState(int state, CL_Pointf vel, CL_Pointf acc)
{
	assert(state < state_Count);

	m_vel = vel;
	m_acc = acc;

	if (state != getStateNo())
	{
		setStateNo(state);
		getSprite().restart();
	}
}

void Player::update_Stand(const LevelCtx &ctx, int posFlags)
{
	// SBA-effects:
	if (!posFlags)
	{ return enterState(state_Jump, m_vel, a_free); }

	// left-key:
	if (ctx.keys.get_keycode(CL_KEY_A))
	{ return enterState(state_Walk, -v_walk, a_zero); }

	// right-key:
	if (ctx.keys.get_keycode(CL_KEY_D))
	{ return enterState(state_Walk, v_walk, a_zero); }

	// up-key:
	if (ctx.keys.get_keycode(CL_KEY_W))
	{
		if (m_action)
		{ return enterAction(ctx); }
		
		else if (posFlags & pf_OnStairs)
		{ return enterState(state_Climb, -v_climb, a_zero); }
		
		else
		{ return enterState(state_Jump, -v_jump, a_free); }
	}

	// down-key:
	if (ctx.keys.get_keycode(CL_KEY_S))
	{
		if (posFlags & pf_TopStairs)
		{ return enterState(state_Climb, v_climb, a_zero); }

		if (posFlags & pf_OnStairs)
		{ return enterState(state_Climb, v_climb, a_zero); }
	}

	// mouse-left:
	if (ctx.mouse.get_keycode(CL_MOUSE_LEFT))
	{ return enterState(state_Strike, v_zero, a_zero); }

	// mouse-right:
	if (ctx.mouse.get_keycode(CL_MOUSE_RIGHT))
	{ return enterState(state_Shield, v_zero, a_zero); }
}

void Player::update_Walk(const LevelCtx &ctx, int posFlags)
{
	// SBA-effects:
	if (!posFlags)
	{ return enterState(state_Jump, m_vel, a_free); }

	// mouse-left:
	if (ctx.mouse.get_keycode(CL_MOUSE_LEFT))
	{ return enterState(state_Pierce, m_vel * 2.0f - v_pier, a_free); }

	// mouse-right:
	if (ctx.mouse.get_keycode(CL_MOUSE_RIGHT))
	{ return enterState(state_Shield, v_zero, a_zero); }

	// up key:
	if (ctx.keys.get_keycode(CL_KEY_W))
	{
		if (m_action)
		{ return enterAction(ctx); }

		else if (!(posFlags & pf_OnStairs))
		{ return enterState(state_Jump, CL_Pointf(m_vel.x, -v_leap.y), a_free); }
	}
	
	// left key:
	if (ctx.keys.get_keycode(CL_KEY_A))
	{ return enterState(state_Walk, -v_walk, a_zero); }

	// right-key:
	else if (ctx.keys.get_keycode(CL_KEY_D))
	{ return enterState(state_Walk, v_walk, a_zero); }
	
	// no direction:
	else
	{ return enterState(state_Stand, v_zero, a_zero); }
}

void Player::update_Jump(const LevelCtx &ctx, int posFlags)
{
	// SBA-effects:
	if (posFlags & pf_OnGround)
	{ return enterState(state_Stand, v_zero, a_zero); }

	// mouse-left:
	if (ctx.mouse.get_keycode(CL_MOUSE_LEFT))
	{ return enterState(state_Slash, m_vel, m_acc * 2.0f); }

	// left-key:
	if (ctx.keys.get_keycode(CL_KEY_A))
	{ return enterState(state_Jump, m_vel, CL_Pointf(-a_jump.x, a_free.y)); }

	// right-key:
	else if (ctx.keys.get_keycode(CL_KEY_D))
	{ return enterState(state_Jump, m_vel, CL_Pointf(a_jump.x, a_free.y)); }

	// no direction:
	else 
	{ return enterState(state_Jump, m_vel, a_free); }
}

void Player::update_Climb(const LevelCtx &ctx, int posFlags)
{
	// SBA-effects:
	if (!posFlags)
	{ return enterState(state_Jump, m_vel, a_free); }

	if ((posFlags & pf_OnGround) && (posFlags & pf_OnStairs))
	{ return enterState(state_Stand, v_zero, a_zero); }

	// up-key:
	if (ctx.keys.get_keycode(CL_KEY_W))
	{
		if (posFlags & pf_OnStairs)
		{ return enterState(state_Climb, -v_climb, a_zero); }
	}

	// down-key:
	else if (ctx.keys.get_keycode(CL_KEY_S))
	{
		if (posFlags & pf_OnStairs)
		{ return enterState(state_Climb, v_climb, a_zero); }
	}

	else
	{ return enterState(state_Stand, v_zero, a_zero); }
}  

void Player::update_Shield(const LevelCtx &ctx, int posFlags)
{
	if (!ctx.mouse.get_keycode(CL_MOUSE_RIGHT))
	{ return enterState(state_Stand, v_zero, a_zero); }
}

void Player::update_Pierce(const LevelCtx &ctx, int posFlags)
{
	if (posFlags & pf_OnGround)
	{ return enterState(state_Stand, v_zero, a_zero); }	
	
	if (getSprite().is_finished())
	{ return enterState(state_Jump, m_vel, a_free); }
}

void Player::update_Slash(const LevelCtx &ctx, int posFlags)
{
	if (posFlags & pf_OnGround)
	{ return enterState(state_Stand, v_zero, a_zero); }	
	
	if (getSprite().is_finished())
	{ return enterState(state_Jump, m_vel, a_free); }
}

void Player::update_Strike(const LevelCtx &ctx, int posFlags)
{
	if (getSprite().is_finished())
	{ return enterState(state_Stand, v_zero, a_zero); }
}

// action handling:

void Player::setAction(Entity * action)
{ m_action = action; }

bool Player::checkAction(Entity * action)
{ return (m_action == action); }

//************************************************************************************************************************
