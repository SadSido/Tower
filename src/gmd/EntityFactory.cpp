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

	Entity::Ref result = createEntity(type, name, plist);

	// set common attributes:

	const CL_Sizef size = readSize(node) / tilesz;
	const CL_Pointf pos = readPoint(node) / tilesz;

	result->setPos(pos);
	result->setSize(size);

	return result;
}

//************************************************************************************************************************

Entity::Ref createEntity(CL_String type, CL_String name, const CL_DomNodeList &plist)
{
	if (type == "DialogEntity")
	{ return Entity::Ref(new DialogEntity(name, plist)); }

	if (type == "AreaEntity")
	{ return Entity::Ref(new AreaEntity(name, plist)); }

	if (type == "SpawnEntity")
	{ return Entity::Ref(new SpawnEntity(name, plist)); }

	if (type == "RainEffect")
	{ return Entity::Ref(new RainEffect(name, plist)); }

	assert(false);
	return Entity::Ref();
}

//************************************************************************************************************************
