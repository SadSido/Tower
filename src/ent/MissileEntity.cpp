// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for the shootable projectiles

#include "MissileEntity.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include <assert.h>

//************************************************************************************************************************

// c-tors and d-tors:

MissileEntity::MissileEntity(CL_String sprite)
: m_bounces(1), m_sprite(sprite)
{
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
		m_vel.x = (moveTest.type == th_Vertical) ? - m_vel.x : + m_vel.x;
		m_vel.y = (moveTest.type == th_Horizontal) ? - m_vel.y : + m_vel.y;
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

//************************************************************************************************************************

