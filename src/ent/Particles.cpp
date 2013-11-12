// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for rendering patricle systems

#include "Particles.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************

namespace
{
	
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

ParticleSystem::ParticleSystem(const Databags &data, const CL_String &name)
: m_partcount(0), m_infinite(false), m_tospawn(0.0f), m_solver(freefallSolver)
{
	auto bag = data.find(name)->second;

	m_interval  = bag->get<Range>("interval");
	m_lifetime  = bag->get<Range>("lifetime");
	m_velocity  = bag->get<Range>("velocity");
	m_direction = bag->get<Range>("direction");
	m_sprName   = bag->get<CL_String>("sprite");
	
	CL_String type = bag->get<CL_String>("track_type");
	m_solver = getNamedSolver(type);

	if (bag->has<bool>("infinite")) 
	{ m_infinite = bag->get<bool>("infinite"); }

	if (!m_infinite)
	{ m_partcount = bag->get<int>("count"); }

	assert(m_infinite || m_partcount);
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
		part.vel  = getRandomVec(m_direction, m_velocity);
		
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
