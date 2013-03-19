// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which generates another entities on certain conditions

#ifndef _SpawnEntity_h_
#define _SpawnEntity_h_

#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class SpawnEntity : public Entity
{
public:
	// common constructor for the entities:
	explicit SpawnEntity(const CL_DomNodeList &props);

private:
	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);

private:
	Entity::Ref   m_spawnee;
	Entities::Ref m_entities;

	float m_tospawn;
	float m_interval;
	int   m_limit;
	bool  m_spawning;
};

//************************************************************************************************************************

#endif
