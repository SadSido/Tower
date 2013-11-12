// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: functions to resolve names and types into actual entities

#include "EntityFactory.h"
#include "../util/XmlUtils.h"

#include "../ent/DialogEntity.h"
#include "../ent/AreaEntity.h"
#include "../ent/SpawnEntity.h"
#include "../ent/RainEffect.h"
#include "../ent/Particles.h"
#include "../ent/MonsterTypes.h"

#include <assert.h>

//************************************************************************************************************************

Entity::Ref createEntity(const Databags &data, const CL_DomElement &node, float tilesz)
{
	const CL_String name = node.get_attribute("name");
	const CL_String type = node.get_attribute("type");

	// generate entity by type & name:

	Entity::Ref result = createEntity(data, type, name);

	// set common attributes:

	const CL_Sizef size = readSize(node) / tilesz;
	const CL_Pointf pos = readPoint(node) / tilesz;

	result->setPos(pos);
	result->setSize(size);

	return result;
}

//************************************************************************************************************************

// can be optimized by using maps??

Entity::Ref createEntity(const Databags &data, const CL_String &type, const CL_String &name)
{
	if (type == "dialog_entity")
	{ return Entity::Ref(new DialogEntity(data, name)); }

	if (type == "area_entity")
	{ return Entity::Ref(new AreaEntity(data, name)); }

	if (type == "spawn_entity")
	{ return Entity::Ref(new SpawnEntity(data, name)); }

	if (type == "rain_effect")
	{ return Entity::Ref(new RainEffect(data, name)); }

	if (type == "particle_system")
	{ return Entity::Ref(new ParticleSystem(data, name)); }

	if (type == "plant_monster")
	{ return createPlant(plist); }

	if (type == "walker_monster")
	{ return createWalker(plist); }

	if (type == "guard_monster")
	{ return createGuard(plist); }

	if (type == "footman_monster")
	{ return createFootman(plist); }

	if (type == "sniper_monster")
	{ return createSniper(plist); }

	assert(false);
	return Entity::Ref();
}

//************************************************************************************************************************
