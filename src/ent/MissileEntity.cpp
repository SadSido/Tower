// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for the shootable projectiles

#include "MissileEntity.h"
#include "../gmd/LevelScene.h"
#include "../util/MathUtils.h"
#include <assert.h>

//************************************************************************************************************************

// c-tors and d-tors:

MissileEntity::MissileEntity()
: m_alive(true)
{
}

Entity::Ref MissileEntity::clone()
{
	return Entity::Ref(new MissileEntity(*this));
}

// entity interface:

bool MissileEntity::update(const LevelCtx &ctx, float secs)
{
	/*
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
	*/

	// select and update sprite:
	getSprite().update();
	return m_alive;
}

bool MissileEntity::render(const LevelCtx &ctx)
{
	CL_Sprite & sprite = getSprite();

	CL_Rectf rect = ctx.tilemap->toScreen(m_rect);
	CL_Draw::box(ctx.gc, rect, CL_Colorf(0,255,0));

	auto facing = getFacing();
	auto anchor = (facing > 0.0f) ? rect.get_top_left() : rect.get_top_right();

	sprite.set_scale(facing, 1.0f);
	sprite.set_color(CL_Color::white);
	sprite.draw(ctx.gc, anchor.x, anchor.y);

	return true;
}

void MissileEntity::upload(const LevelCtx &ctx)
{
	/*
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
	*/
}

//************************************************************************************************************************

