// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which allows player to pass between areas

#include "AreaEntity.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include <assert.h>

//************************************************************************************************************************
	
AreaEntity::AreaEntity(const Databags &data, const CL_String &name)
: m_gen(0), m_opened(true)
{
	auto bag = data.find(name)->second;

	m_area  = bag->get<CL_String>("area");
	m_entry = bag->get<CL_String>("entry");
	m_cond	= bag->get<CL_String>("condition");
}

bool AreaEntity::update(const LevelCtx &ctx, float secs)
{
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && isInside && isOpened(ctx))
	{ ctx.player.setAction(this); }

	else if (assigned && !isInside)
	{ ctx.player.setAction(NULL); }

	return true;
}

bool AreaEntity::render(const LevelCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,255));
	return true;
}

void AreaEntity::notify(const LevelCtx &ctx, Notify code)
{
	assert(code == n_DoAction);
	GameScene::Ref dlgScene(new AreaScene(ctx.manager, m_area, m_entry));
	ctx.manager->pushScene(dlgScene);
}

bool AreaEntity::isOpened(const LevelCtx &ctx) const
{
	// maybe require re-checking?
	if (!m_cond.empty() && m_gen != ctx.globals.getGen())
	{
		m_gen = ctx.globals.getGen();
		m_opened = ctx.globals.check(m_cond);
	}

	// returned cached result:
	return m_opened;
}

//************************************************************************************************************************
