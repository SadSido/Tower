// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity which is merely playing a sprite animation

#include "SpriteEntity.h"
#include <assert.h>

//************************************************************************************************************************

namespace
{

enum States
{
	state_True,
	state_False,
	state_Count
};

}

//************************************************************************************************************************

SpriteEntity::SpriteEntity(const Databags &data, const CL_String &name)
: m_gen(0)
{
	auto bag = data.find(name)->second;

	m_condition = bag->get<CL_String>("condition");
	m_sprTrue   = bag->get<CL_String>("sprite_true");
	m_sprFalse  = bag->get<CL_String>("sprite_false");

	// shortcut for non-conditional sprites:

	if (bag->has<CL_String>("sprite"))
	{ 
		assert(m_condition.empty()); 
		m_sprTrue = bag->get<CL_String>("sprite"); 
	}
}

bool SpriteEntity::update(const LevelCtx &ctx, float secs)
{
	// check states:
	checkState(ctx);

	// update sprite if any:
	if (!getSprite().is_null())
	{ getSprite().update(); }

	// never vanish:
	return true;
}

bool SpriteEntity::render(const LevelCtx &ctx)
{
	auto sprite = getSprite();
	if (!sprite.is_null())
	{
		auto anchor = ctx.tilemap->toScreen(m_rect).get_top_left();
		sprite.draw(ctx.gc, anchor.x, anchor.y);
	}

	return true;
}

void SpriteEntity::upload(const LevelCtx &ctx)
{
	SpriteVec & sprites = getSprites();
	sprites.resize(state_Count);

	// load sprites:
	if (!m_sprTrue.empty())
	{ sprites[state_True] = CL_Sprite(ctx.gc, m_sprTrue, &ctx.assets); }

	if (!m_sprFalse.empty())
	{ sprites[state_False] = CL_Sprite(ctx.gc, m_sprFalse, &ctx.assets); }
}

// per-state updates:

void SpriteEntity::enterState(int stateNo)
{
	if (getStateNo() != stateNo)
	{ 
		// apply new state:
		setStateNo(stateNo);

		// restart the sprite:
		if (!getSprite().is_null())
		{ getSprite().restart(); }
	}
}

void SpriteEntity::checkState(const LevelCtx &ctx)
{
	// no global set - no state change:
	if (m_condition.empty()) { return; }

	// check, maybe the global has changed:
	if (m_gen != ctx.globals.getGen())
	{ 
		m_gen = ctx.globals.getGen();
		enterState((ctx.globals.check(m_condition)) ? state_True : state_False);
	}
}

//************************************************************************************************************************
