// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which generates another entities on certain conditions

#include "SpawnEntity.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************
	
SpawnEntity::SpawnEntity(CL_String name, const CL_DomNodeList &props)
: Entity("SpawnEntity", name)
{
	/*
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "Area") 
		{ m_area = prop.get_attribute("value"); }
	
		if (prop.get_attribute("name") == "Entry") 
		{ m_entry = prop.get_attribute("value"); }
	}
	*/
}

bool SpawnEntity::update(const LevelCtx &ctx, float secs)
{
	/*
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && isInside)
	{ ctx.player.setAction(this); }

	else if (assigned && !isInside)
	{ ctx.player.setAction(NULL); }

	*/

	// perform the rest of the update:
	// ...
	return true;
}

bool SpawnEntity::render(const LevelCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,255));
	return true;
}

//************************************************************************************************************************
