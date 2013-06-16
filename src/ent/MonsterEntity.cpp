// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for the game monsters

#include "MonsterEntity.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************

enum States
{
	state_Emerge,
	state_Move,
	state_Wait,
	state_Vanish,
	state_Count,
};

static CL_String getStateName(int state)
{
	switch (state)
	{
	case state_Emerge:  return "_emerge";
	case state_Move:	return "_move";
	case state_Wait:	return "_wait";
	case state_Vanish:	return "_vanish";
	}

	assert(false);
	return CL_String();
};

//************************************************************************************************************************

bool MonsterEntity::update(const LevelCtx &ctx, float secs)
{
	/*
	// dispatch state-based update:
	switch (getStateNo())
	{
	case state_Emerge: { update_Emerge (ctx); break; }
	case state_Move:   { update_Move   (ctx); break; }
	case state_Wait:   { update_Wait   (ctx, secs); break;  } 
	case state_Vanish: { update_Vanish (ctx); break; }
	}

	// resolve movement:
	if (!isRecovering())
	{
		m_vel += m_acc * secs;
		m_rect.translate(m_vel * secs);
		setFacing();
	}

	*/
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
		sprite.set_color(getRecoverColor());
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

//************************************************************************************************************************

