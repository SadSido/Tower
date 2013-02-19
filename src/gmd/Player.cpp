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

}

//************************************************************************************************************************

Player::Player(CL_Pointf pos, CL_Sizef size)
: Entity("player", "player", spr_Count), m_climbing(false), m_action(NULL), m_facing(1.0f)
{
	setPos(pos);
	setSize(size);
}

bool Player::update(const UpdateCtx &ctx, float secs)
{
	// get position flags:
	int posFlags = getPosFlags(ctx);

	// up/down movement:

	if (ctx.keys.get_keycode(CL_KEY_W))
	{ handleUpKey(ctx, posFlags); }

	else if (ctx.keys.get_keycode(CL_KEY_S))
	{ handleDownKey(ctx, posFlags); }

	else
	{ if (m_climbing) m_vel.y = 0.0f; }

	// left/right movement:

	if (ctx.keys.get_keycode(CL_KEY_A))
	{ handleLeftKey(ctx, posFlags); }

	else if (ctx.keys.get_keycode(CL_KEY_D))
	{ handleRightKey(ctx, posFlags); }
	
	else
	{ m_acc.x = 0.0f; if (posFlags & pf_OnGround) m_vel.x = 0.0f; }

	// resolve gravity and movement:

	m_acc.y = (posFlags) ? 0.0f : +12.0f;
	m_vel += m_acc * secs;

	TileChecker check = (m_climbing) ? isBlocking : anyBlocking;
	TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, check);

	if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

	m_rect.translate(moveTest.delta);
	m_facing = (m_vel.x) ? (m_vel.x > 0.0f) ? +1.0f : -1.0f : m_facing;

	// discard action flags if any:
	if (!(posFlags & pf_OnStairs)) { m_climbing = false; }

	// select and update sprite:

	bool mustUpdate = true;
	CL_Sprite & sprite = getSprite();

	if (m_climbing) 
	{ 
		setSpriteNo(spr_Walk);
		mustUpdate = (m_vel.y != 0.0f);
	} 
	else if (posFlags & pf_OnGround) 
	{ 
		setSpriteNo(spr_Walk);
		mustUpdate = (m_vel.x != 0.0f);
	} 
	else
	{
		setSpriteNo(spr_Jump);
		mustUpdate = true;
	}

	if (!sprite.is_null())
	{ (mustUpdate) ? sprite.update(secs * 1000) : sprite.restart(); }

	return true;
}

bool Player::render(const RenderCtx &ctx)
{
	CL_Sprite & sprite = getSprite();

	if (sprite.is_null())
	{ sprite = CL_Sprite(ctx.gc, getSpriteID(getSpriteNo()), &ctx.assets); }


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

// tilemap check helpers:

int Player::getPosFlags(const UpdateCtx &ctx)
{
	int result = 0;
	
	if (standOnGround(ctx.tilemap, m_rect))  { result |= pf_OnGround; }
	if (standOnStairs(ctx.tilemap, m_rect))  { result |= pf_OnStairs; }
	if (standTopStairs(ctx.tilemap, m_rect)) { result |= pf_TopStairs; }
	
	return result;
}

CL_String Player::getSpriteID (int sNo)
{
	switch (sNo)
	{
	case spr_Walk: return "arteus_walk";
	case spr_Jump: return "arteus_jump";
	}

	assert(false);
	return CL_String();
}

// input processing helpers:

void Player::handleUpKey(const UpdateCtx &ctx, int posFlags)
{
	if (m_action)
	{
		m_action->notify(ctx, n_DoAction);
		m_action = NULL;
		return;
	}

	if (posFlags & pf_OnStairs)
	{
		m_vel.y = -6.0f;
		m_climbing = true;
	}
	else if (posFlags & pf_OnGround)
	{
		m_vel.y = abs(m_vel.x) > 0.0f ? -6.0f : -9.0f;
	}
}

void Player::handleDownKey(const UpdateCtx &ctx, int posFlags)
{
	if (posFlags & pf_TopStairs)
	{
		m_vel.y = +6.0f;
		m_climbing = true;
	}
	else if (posFlags & pf_OnStairs)
	{
		m_vel.y = +6.0f;
	}
}

void Player::handleLeftKey(const UpdateCtx &ctx, int posFlags)
{
	if (!posFlags)
	{
		m_acc.x = -6.0f;
	} 
	else 
	{
		m_vel.x = -4.0f;
	}
}

void Player::handleRightKey(const UpdateCtx &ctx, int posFlags)
{
	if (!posFlags)
	{
		m_acc.x = +6.0f;
	} 
	else 
	{
		m_vel.x = +4.0f;
	}
}

// action handling:

void Player::setAction(Entity * action)
{ m_action = action; }

bool Player::checkAction(Entity * action)
{ return (m_action == action); }

//************************************************************************************************************************
