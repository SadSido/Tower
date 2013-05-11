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
	CL_Pointf acc;

	float life;
	float freq;
};

//************************************************************************************************************************

class ParticleSystem : public Entity
{
public:
	// common constructor for the entities:
	explicit ParticleSystem(const CL_DomNodeList &props);

private:
	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);
	virtual void upload(const LevelCtx &ctx);

private:
	// properties:
	int m_moveType;
	int m_partLimit;

	Range m_interval;
	Range m_lifetime;
	float m_tospawn;

	CL_String m_sprName;
	CL_Sprite m_sprite;

	std::list<Particle> m_parts;
};

//************************************************************************************************************************

#endif
