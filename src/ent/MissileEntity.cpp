// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for the shootable projectiles

#include "MissileEntity.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************

// c-tors and d-tors:

MissileEntity::MissileEntity(const CL_DomNodeList &props)
: m_bounces(1), m_homing(false)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "shoot_prefix") 
		{ m_sprite = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "shoot_bounce") 
		{ m_bounces = prop.get_attribute_int("value"); }

		if (prop.get_attribute("name") == "shoot_vel") 
		{ m_lockVel = readPoint(prop, "value"); }

		if (prop.get_attribute("name") == "shoot_acc") 
		{ m_lockAcc = readPoint(prop, "value"); }
	}
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
	m_rect.translate(moveTest.delta);

	// check collision with the tilemap
	if (moveTest.type != th_None)
	{
		m_bounces -= 1;
		m_vel.x = (moveTest.type == th_Horizontal) ? - m_vel.x : + m_vel.x;
		m_vel.y = (moveTest.type == th_Vertical)   ? - m_vel.y : + m_vel.y;
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

