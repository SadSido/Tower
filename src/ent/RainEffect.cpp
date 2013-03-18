// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: effect to show the neverending rain

#include "RainEffect.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include <assert.h>

//************************************************************************************************************************
	
RainEffect::RainEffect(CL_String name, const CL_DomNodeList &props)
: Entity("RainEffect", name)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();
	}

}

bool RainEffect::update(const LevelCtx &ctx, float secs)
{
	/*
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && isInside)
	{ ctx.player.setAction(this); }

	else if (assigned && !isInside)
	{ ctx.player.setAction(NULL); }

	// perform the rest of the update:
	// ...
	*/
	return true;
}

bool RainEffect::render(const LevelCtx &ctx)
{
	// CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,255));
	return true;
}

//************************************************************************************************************************
