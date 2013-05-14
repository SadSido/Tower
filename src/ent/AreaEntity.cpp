// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which allows player to pass between areas

#include "AreaEntity.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include <assert.h>

//************************************************************************************************************************
	
AreaEntity::AreaEntity(const CL_DomNodeList &props)
: m_gen(0), m_cond(true)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "area") 
		{ m_area = prop.get_attribute("value"); }
	
		if (prop.get_attribute("name") == "entry") 
		{ m_entry = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "condition") 
		{ m_condition = prop.get_attribute("value"); }
	}
}

bool AreaEntity::update(const LevelCtx &ctx, float secs)
{
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && isInside && checkCondition(ctx))
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

bool AreaEntity::checkCondition(const LevelCtx &ctx)
{
	// maybe require re-checking?
	if (!m_condition.empty() && m_gen != ctx.globals.getGen())
	{
		m_gen = ctx.globals.getGen();
		m_cond = ctx.globals.check(m_condition);
	}

	// returned cached result:
	return m_cond;
}

//************************************************************************************************************************
