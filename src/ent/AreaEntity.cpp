// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which allows player to pass between areas

#include "AreaEntity.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include <assert.h>

//************************************************************************************************************************
	
AreaEntity::AreaEntity(CL_String name, const CL_DomNodeList &props)
: Entity("AreaEntity", name)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "Area") 
		{ m_area = prop.get_attribute("value"); }
	
		if (prop.get_attribute("name") == "Entry") 
		{ m_entry = prop.get_attribute("value"); }
	}

}

bool AreaEntity::update(const LevelCtx &ctx, float secs, int msecs)
{
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && isInside)
	{ ctx.player.setAction(this); }

	else if (assigned && !isInside)
	{ ctx.player.setAction(NULL); }

	// perform the rest of the update:
	// ...
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

//************************************************************************************************************************
