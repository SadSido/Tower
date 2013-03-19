// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: functions to resolve names and types into actual entities

#include "EntityFactory.h"
#include "../util/XmlUtils.h"

#include "../ent/DialogEntity.h"
#include "../ent/AreaEntity.h"
#include "../ent/SpawnEntity.h"
#include "../ent/RainEffect.h"

#include <assert.h>

//************************************************************************************************************************

Entity::Ref createEntity(CL_DomElement node, float tilesz)
{
	const CL_String name = node.get_attribute("name");
	const CL_String type = node.get_attribute("type");
	
	CL_DomElement  props = node.get_first_child().to_element();
	CL_DomNodeList plist = props.get_elements_by_tag_name("property");

	// generate entity by type:

	Entity::Ref result = createEntity(type, plist);

	// set common attributes:

	const CL_Sizef size = readSize(node) / tilesz;
	const CL_Pointf pos = readPoint(node) / tilesz;

	result->setPos(pos);
	result->setSize(size);

	return result;
}

//************************************************************************************************************************

Entity::Ref createEntity(CL_String type, const CL_DomNodeList &plist)
{
	if (type == "dialog_entity")
	{ return Entity::Ref(new DialogEntity(plist)); }

	if (type == "area_entity")
	{ return Entity::Ref(new AreaEntity(plist)); }

	if (type == "spawn_entity")
	{ return Entity::Ref(new SpawnEntity(plist)); }

	if (type == "rain_effect")
	{ return Entity::Ref(new RainEffect(plist)); }

	assert(false);
	return Entity::Ref();
}

//************************************************************************************************************************
