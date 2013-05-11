// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for rendering patricle systems

#include "Particles.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************
	
ParticleSystem::ParticleSystem(const CL_DomNodeList &props)
: m_moveType(0), m_partLimit(0), m_tospawn(0.0f)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "interval")
		{ m_interval = readRange(prop, "value"); }

		if (prop.get_attribute("name") == "lifetime")
		{ m_lifetime = readRange(prop, "value"); }

		if (prop.get_attribute("name") == "sprite")
		{ m_sprName = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "move_type")
		{ m_moveType = prop.get_attribute_int("value"); }

		if (prop.get_attribute("name") == "particle_limit")
		{ m_partLimit = prop.get_attribute_int("value"); }
	}
}

bool ParticleSystem::update(const LevelCtx &ctx, float secs)
{
	// update the sprite:
	m_sprite.update();

	// update the particles:
	for (auto it = m_parts.begin(); it != m_parts.end(); /**/)
	{
		auto cur = it ++;
		
		cur->life -= secs;
		cur->vel  += cur->acc * secs;
		cur->pos  += cur->vel * secs;

		if (cur->life <= 0.0f) { m_parts.erase(cur); }
	}

	// decrement spawning timeout:
	m_tospawn = max(0.0f, m_tospawn - secs);
	
	// maybe spawn new particle:
	if (m_tospawn == 0.0f && m_partLimit)
	{
		Particle part;

		part.life = m_lifetime.random();
		part.freq = 1.0f / part.life;
		part.pos  = m_rect.get_center();
		part.vel  = CL_Pointf((float(rand() % 16 - 8), float(rand() % 16 - 8)));
		part.acc  = CL_Pointf(0.0f, 20.0f);
		
		m_parts.push_back(part);

		m_tospawn = m_interval.random();
		m_partLimit = max(0, m_partLimit - 1);
	}

	// alive until we have particles:
	return (m_partLimit || !m_parts.empty());
}

bool ParticleSystem::render(const LevelCtx &ctx)
{
	for (auto it = m_parts.begin(); it != m_parts.end(); ++ it)
	{
		const float alpha = alphaInterpolateParabolic(it->life, it->freq); 

		auto pos = ctx.tilemap->toScreen(it->pos);
		m_sprite.set_color(CL_Colorf(1.0f, 1.0f, 1.0f, alpha));
		m_sprite.draw(ctx.gc, pos.x, pos.y);
	}

	return true;
}

void ParticleSystem::upload(const LevelCtx &ctx)
{
	if (!m_sprName.empty())
	{ m_sprite = CL_Sprite(ctx.gc, m_sprName, &ctx.assets); }
}

//************************************************************************************************************************
