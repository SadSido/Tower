// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for rendering patricle systems

#include "Particles.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************

namespace
{
	
// select a random point from the range of directions and amplitudes:

CL_Pointf getRandomPoint(const Range &direction, const Range &amplitude)
{
	const float dir = direction.random();
	const float amp = amplitude.random();

	// it is more comfortable to have 90 degrees as "upwards":
	return CL_Pointf(cos(degToRad(dir)), -1.0f * sin(degToRad(dir))) * amp;
}

// some predefined particle solvers:

void linearSolver(Particle &part, float secs)
{ 
	part.pos += part.vel * secs; 
}

void freefallSolver(Particle &part, float secs)
{
	const CL_Pointf acc = CL_Pointf(0.0f, 20.0f);

	part.vel += acc * secs;
	part.pos += part.vel  * secs;
}

void helixSolver(Particle &part, float secs)
{
	const float factor = 6.0f;
	const CL_Pointf acc = CL_Pointf(factor * sin(factor * part.life), 0.0f);

	part.vel += acc * secs;
	part.pos += part.vel  * secs;
}

// select a solver by name:

ParticleSystem::SolverFn getNamedSolver(const CL_String &name)
{
	if (name == "linear")   { return linearSolver; }
	if (name == "freefall") { return freefallSolver; }
	if (name == "helix")    { return helixSolver; }

	assert(false);
	return NULL;
}

}

//************************************************************************************************************************

ParticleSystem::ParticleSystem(const CL_DomNodeList &props)
: m_partcount(0), m_infinite(false), m_tospawn(0.0f), m_solver(freefallSolver)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "interval")
		{ m_interval = readRange(prop, "value"); }

		if (prop.get_attribute("name") == "lifetime")
		{ m_lifetime = readRange(prop, "value"); }

		if (prop.get_attribute("name") == "velocity")
		{ m_velocity = readRange(prop, "value"); }

		if (prop.get_attribute("name") == "direction")
		{ m_direction = readRange(prop, "value"); }

		if (prop.get_attribute("name") == "sprite")
		{ m_sprName = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "count")
		{ 
			const CL_String value = prop.get_attribute("value");

			m_infinite  = (value == "infinite");
			m_partcount = (m_infinite) ? 0 : CL_StringHelp::text_to_int(value);
		}

		if (prop.get_attribute("name") == "track_type")
		{ m_solver = getNamedSolver(prop.get_attribute("value")); }
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
		
		(*m_solver)(*cur, secs);

		cur->life -= secs;
		if (cur->life <= 0.0f) { m_parts.erase(cur); }
	}

	// decrement spawning timeout:
	m_tospawn = max(0.0f, m_tospawn - secs);
	
	// maybe spawn new particle:
	if (m_tospawn == 0.0f && canSpawn())
	{
		Particle part;

		part.life = m_lifetime.random();
		part.freq = 1.0f / part.life;
		part.pos  = m_rect.get_center();
		part.vel  = getRandomPoint(m_direction, m_velocity);
		
		m_parts.push_back(part);

		m_tospawn = m_interval.random();
		m_partcount = max(0, m_partcount - 1);
	}

	// alive until we have particles:
	return (canSpawn() || !m_parts.empty());
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
