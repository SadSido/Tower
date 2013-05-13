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

SpriteEntity::SpriteEntity(const CL_DomNodeList &props)
: m_gen(0)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "global")
		{ m_global = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "sprite_true")
		{ m_sprTrue = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "sprite_false")
		{ m_sprFalse = prop.get_attribute("value"); }
	}
}

bool SpriteEntity::update(const LevelCtx &ctx, float secs)
{
	// check states:
	switch (getStateNo())
	{
	case state_True:  { update_True  (ctx); break; }
	case state_False: { update_False (ctx); break; }
	}

	// update sprite if any:
	auto sprite = getSprite();
	if (!sprite.is_null()) { sprite.update(); }

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
	setStateNo(stateNo);

	auto sprite = getSprite();
	if (!sprite.is_null()) { sprite.restart(); }
}

void SpriteEntity::update_True(const LevelCtx &ctx)
{
	// no global set - no state change:
	if (m_global.empty()) { return; }

	// check, maybe the global has changed:
	if (m_gen != ctx.globals.getGen() && !ctx.globals.check(m_global))
	{ 
		m_gen = ctx.globals.getGen();
		enterState(state_False); 
	}
}

void SpriteEntity::update_False(const LevelCtx &ctx)
{
	// unreachable without global:
	assert(!m_global.empty());

	// check, maybe the global has changed:
	if (m_gen != ctx.globals.getGen() && ctx.globals.check(m_global))
	{ 
		m_gen = ctx.globals.getGen();
		enterState(state_True); 
	}
}

//************************************************************************************************************************
