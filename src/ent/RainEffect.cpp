// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: effect to show the neverending rain

#include "RainEffect.h"
#include "../sys/GameManager.h"
#include "../gmd/AreaScene.h"
#include "../util/XmlUtils.h"
#include "../util/MathUtils.h"
#include <assert.h>

//************************************************************************************************************************
	
RainEffect::RainEffect(const Databags &data, const CL_String &name)
: m_interval(0.0f), m_lifetime(0.0f), m_lifefreq(0.0f), m_tospawn(0.0f)
{
	auto bag = data.find(name)->second;

	m_interval = bag->get<float>("interval");
	m_lifetime = bag->get<float>("lifetime");
	m_lifefreq = 1.0f / m_lifetime;
	m_droplen  = bag->get<CL_Pointf>("drop_len");
	m_dropvel  = bag->get<CL_Pointf>("drop_vel");
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

		const int newx = rand() % ctx.gc.get_size().width;
		const int newy = rand() % ctx.gc.get_size().height / 2;

		Raindrop drop = { CL_Pointf((float)newx, (float)newy), m_dropvel, m_lifetime };
		m_drops.push_back(drop);
	}

	return true;
}

bool RainEffect::render(const LevelCtx &ctx)
{
	for (auto it = m_drops.begin(); it != m_drops.end(); ++ it)
	{
		const float alpha = alphaInterpolateLinear(it->life, m_lifefreq); 
		CL_Draw::line(ctx.gc, it->pos, it->pos + m_droplen, CL_Colorf(0.5f, 0.5f, 0.5f, alpha));
	}

	return true;
}

//************************************************************************************************************************
