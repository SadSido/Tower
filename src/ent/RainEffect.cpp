// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: effect to show the neverending rain

#include "RainEffect.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************
	
RainEffect::RainEffect(const CL_DomNodeList &props)
: m_interval(0.0f), m_lifetime(0.0f), m_tospawn(0.0f)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "interval")
		{ m_interval = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "lifetime")
		{ m_lifetime = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "drop_len")
		{ m_droplen = readPoint(prop, "value"); }

		if (prop.get_attribute("name") == "drop_vel")
		{ m_dropvel = readPoint(prop, "value"); }
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
		int newy = rand() % ctx.gc.get_size().height / 2;

		Raindrop drop = { CL_Pointf(newx, newy), m_dropvel, m_lifetime };
		m_drops.push_back(drop);
	}

	return true;
}

bool RainEffect::render(const LevelCtx &ctx)
{
	for (auto it = m_drops.begin(); it != m_drops.end(); ++ it)
	{
		const float alpha = 1.0f - abs(2.0f * it->life / m_lifetime - 1.0f); 
		CL_Draw::line(ctx.gc, it->pos, it->pos + m_droplen, CL_Colorf(0.5f, 0.5f, 0.5f, alpha));
	}

	return true;
}

//************************************************************************************************************************
