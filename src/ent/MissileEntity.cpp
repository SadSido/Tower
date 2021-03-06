// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for the shootable projectiles

#include "MissileEntity.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************

namespace
{

// geometry helpers:

bool intersects(const CL_Rectf &rect, const CL_Pointf &p1, const CL_Pointf &p2)
{
	bool result = false; CL_LineSegment2f(p1, p2).clip(rect, result);
	return result;
}

void bounceOffRect(CL_Rectf &rect, CL_Pointf &vel, const CL_Rectf &solid)
{
	const bool higher = (rect.get_center().y > solid.get_center().y);
	const float left  = (vel.x > 0.0f) ? solid.left - rect.get_width() : solid.right;
	
	rect.translate(left - rect.left, 0);
	
	vel.x *= - 1.00f;
	vel.y *= higher ? - 1.00f : + 1.00f;
}

}

//************************************************************************************************************************

// c-tors and d-tors:

MissileEntity::MissileEntity(const Databags &data, const CL_String &name)
: m_damage(1.0f), m_bounces(1), m_homing(false)
{
	auto bag = data.find(name)->second;

	m_sprite	= bag->get<CL_String>("sprite");
	m_bounces	= bag->get<int>("bounce");
	m_lockVel	= bag->get<CL_Pointf>("vel");
	m_lockAcc	= bag->get<CL_Pointf>("acc");
	m_damage	= bag->get<float>("damage");
}

Entity::Ref MissileEntity::clone()
{
	return Entity::Ref(new MissileEntity(*this));
}

// entity interface:

bool MissileEntity::update(const LevelCtx &ctx, float secs)
{
	m_vel += m_acc * secs;
	setFacing();

	TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, anyBlocking);
	
	// series of player tests:
	const bool testColl = intersects(ctx.player.getShieldRect(), getCenter(), getCenter() + moveTest.delta);
	const bool testHit  = intersects(ctx.player.getRect(), getCenter(), getCenter() + moveTest.delta);
	const bool testFace = (ctx.player.getFacing() > 0.0f) == (m_vel.x < 0.0f);

	// handle collision with the shield:
	if (testColl && testFace)
	{
		-- m_bounces;
		bounceOffRect(m_rect, m_vel, ctx.player.getShieldRect());
	}	
	// handle collision with the player:
	else if (testHit)
	{
		-- m_bounces;
		bounceOffRect(m_rect, m_vel, ctx.player.getRect());
		ctx.player.applyDamage(m_damage);
	}
	// handle collision with the tilemap
	else 
	{
		m_rect.translate(moveTest.delta);		
		
		if (moveTest.type != th_None)
		{
			m_bounces -= 1;
			m_vel.x = (moveTest.type == th_Horizontal) ? - m_vel.x : + m_vel.x;
			m_vel.y = (moveTest.type == th_Vertical)   ? - m_vel.y : + m_vel.y;
		}
	}

	// select and update sprite:
	getSprite().update();

	// check for exceeding bounces limit;
	return (m_bounces > 0);
}

bool MissileEntity::render(const LevelCtx &ctx)
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

void MissileEntity::upload(const LevelCtx &ctx)
{
	SpriteVec & sprites = getSprites();
	sprites.resize(1);

	sprites[0] = CL_Sprite(ctx.gc, m_sprite, &ctx.assets);
}

void MissileEntity::notify(const LevelCtx &ctx, Notify code)
{
	assert(code == n_LockTarget);

	// resolve player direction:
	const float dir = (ctx.player.getCenter().x < getRect().get_center().x) ? -1.0f : +1.0f;

	// assign movement parameters:
	m_vel = CL_Pointf(m_lockVel.x * dir, m_lockVel.y);
	m_acc = m_lockAcc;
}

//************************************************************************************************************************

