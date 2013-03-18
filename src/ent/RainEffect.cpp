// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: effect to show the neverending rain

#include "RainEffect.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include <assert.h>

//************************************************************************************************************************
	
RainEffect::RainEffect(CL_String name, const CL_DomNodeList &props)
: Entity("RainEffect", name), m_interval(0.0f), m_lifetime(0.0f), m_tospawn(0.0f)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "interval")
		{ m_interval = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "lifetime")
		{ m_lifetime = prop.get_attribute_float("value"); }
	}
}

bool RainEffect::update(const LevelCtx &ctx, float secs)
{
	// update the raindrops:
	for (auto it = m_drops.begin(); it != m_drops.end(); /**/)
	{
		auto cur = it ++;
		
		cur->life -= secs;		
		cur->pos  += cur->vel * secs;

		if (cur->life <= 0.0f) { m_drops.erase(cur); }
	}

	// check spawning interval:
	m_tospawn -= secs;
	if (m_tospawn <= 0.0f)
	{
		m_tospawn = m_interval;

		int newx = rand() % ctx.gc.get_size().width;
		int newy = rand() % ctx.gc.get_size().height;

		Raindrop drop = { CL_Pointf(newx, newy), CL_Pointf(0,200), m_lifetime };
		m_drops.push_back(drop);
	}

	return true;
}

bool RainEffect::render(const LevelCtx &ctx)
{
	for (auto it = m_drops.begin(); it != m_drops.end(); ++ it)
	{
		CL_Draw::circle(ctx.gc, it->pos, 3.0f, CL_Colorf::gray10);
	}

	return true;
}

//************************************************************************************************************************
