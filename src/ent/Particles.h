// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for rendering patricle systems

#ifndef _Particles_h_
#define _Particles_h_

#include "../gmd/LevelScene.h"
#include "../gmd/Entities.h"
#include "../util/MathUtils.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

struct Particle
{
	CL_Pointf pos;
	CL_Pointf vel;

	float life;
	float freq;
};

//************************************************************************************************************************

class ParticleSystem : public Entity
{

public:
	// function, determining the movement:
	typedef void (*SolverFn)(Particle&, float);

	// common constructor for the entities:
	explicit ParticleSystem(const Databags &data, const CL_String &name);

private:
	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);
	virtual void upload(const LevelCtx &ctx);

	// minor helpers:
	inline bool canSpawn() const
	{ return m_infinite || m_partcount; }

private:
	// properties:
	bool  m_infinite;
	int   m_partcount;
	Range m_interval;
	Range m_lifetime;
	Range m_direction;
	Range m_velocity;
	float m_tospawn;

	CL_String m_sprName;
	CL_Sprite m_sprite;

	SolverFn m_solver;
	std::list<Particle> m_parts;
};

//************************************************************************************************************************

#endif
