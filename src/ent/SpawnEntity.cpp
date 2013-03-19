// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which generates another entities on certain conditions

#include "SpawnEntity.h"
#include "../gmd/LevelScene.h"
#include <assert.h>

//************************************************************************************************************************
	
SpawnEntity::SpawnEntity(const CL_DomNodeList &props)
: m_spawning(false)
{
	CL_String spawnee;
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "Spawnee") 
		{ spawnee = prop.get_attribute("value"); }

		if (prop.get_attribute("name") == "Interval") 
		{ m_interval = prop.get_attribute_float("value"); }

		if (prop.get_attribute("name") == "Limit") 
		{ m_limit = prop.get_attribute_int("value"); }
	}

	// create spawnee pattern:
	m_spawnee = createEntity(spawnee, props);

	// create list of entities:
	m_entities = Entities::Ref(new Entities());
}

// entity lifecycle:

bool SpawnEntity::update(const LevelCtx &ctx, float secs)
{
	// update the sub-entities:
	for (auto it = m_entities->begin(); it != m_entities->end(); /**/)
	{
		auto cur = it ++;
		bool res = (*cur)->doUpdate(ctx, secs);

		// drop the entity, which returned false:
		if (!res) { m_entities->erase(cur); }
	}

	// handle spawning of new ones:
	if (m_entities->size() < m_limit)
	{
		if (m_spawning)
		{
			// countdown to spawning:
			m_tospawn = max(0.0f, m_tospawn - secs);
			
			// spawn new entity:
			if (!m_tospawn)
			{
				m_spawning = false;
				m_entities->push_back(m_spawnee->clone());
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

//************************************************************************************************************************
