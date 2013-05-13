// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity which is merely playing a sprite animation

#include "SpriteEntity.h"
#include <assert.h>

//************************************************************************************************************************

SpriteEntity::SpriteEntity(const CL_DomNodeList &props)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		//if (prop.get_attribute("name") == "track_type")
		//{ m_solver = getNamedSolver(prop.get_attribute("value")); }
	}
}

bool SpriteEntity::update(const LevelCtx &ctx, float secs)
{
	return true;
}

bool SpriteEntity::render(const LevelCtx &ctx)
{
	return true;
}

void SpriteEntity::upload(const LevelCtx &ctx)
{
}

//************************************************************************************************************************
