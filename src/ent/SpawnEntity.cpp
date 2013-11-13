// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which generates another entities on certain conditions

#include "SpawnEntity.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************
	
SpawnEntity::SpawnEntity(const Databags &data, const CL_String &name)
: m_spawning(false)
{
	auto bag = data.find(name)->second;

	CL_String spawntype = bag->get<CL_String>("spawn_type");
	CL_String spawnname = bag->get<CL_String>("spawn_name");
	
	m_interval = bag->get<float>("interval");
	m_limit    = bag->get<int>("limit");

	// create spawnee pattern:
	m_spawnee = createEntity(data, spawntype, spawnname);

	// create list of entities:
	m_entities = Entities::Ref(new Entities());
}

// entity lifecycle:

bool SpawnEntity::update(const LevelCtx &ctx, float secs)
{
	// upload spawnee once:
	m_spawnee->doUpload(ctx);

	// update the sub-entities:
	for (auto it = m_entities->begin(); it != m_entities->end(); /**/)
	{
		auto cur = it ++;
		bool res = (*cur)->doUpdate(ctx, secs);

		// drop the entity, which returned false:
		if (!res) { m_entities->erase(cur); }
	}

	// handle spawning of new ones:
	if ((int)m_entities->size() < m_limit)
	{
		if (m_spawning)
		{
			// countdown to spawning:
			m_tospawn = max(0.0f, m_tospawn - secs);
			
			// spawn new entity:
			if (!m_tospawn)
			{
				m_spawning = false;
				m_entities->push_back(spawn());
			}
		}
		else
		{
			// start spawning interval:
			m_spawning = true;
			m_tospawn  = m_interval;
		}
	}

	// always true:
	return true;
}

bool SpawnEntity::render(const LevelCtx &ctx)
{
	// render the sub-entities:
	for (auto it = m_entities->begin(); it != m_entities->end(); ++ it)
	{ (*it)->doRender(ctx); }

	return true;
}

// spawning new guys:

Entity::Ref SpawnEntity::spawn()
{
	auto result = m_spawnee->clone();
	assert(result);

	result->setPos(m_rect.get_top_left());
	result->setSize(m_rect.get_size());

	return result;
}

//************************************************************************************************************************
