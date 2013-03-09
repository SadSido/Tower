// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: functions to resolve names and types into actual entities

#include "EntityFactory.h"
#include "../util/XmlUtils.h"

#include "../ent/DialogEntity.h"
#include "../ent/AreaEntity.h"
#include "../ent/SpawnEntity.h"

//************************************************************************************************************************

Entity::Ref createEntity(CL_DomElement node, float tilesz)
{
	const CL_String name = node.get_attribute("name");
	const CL_String type = node.get_attribute("type");
	
	CL_DomElement  props = node.get_first_child().to_element();
	CL_DomNodeList plist = props.get_elements_by_tag_name("property");

	Entity::Ref result;
	
	// generate entity by type:

	if (type == "DialogEntity")
	{ result = Entity::Ref(new DialogEntity(name, plist)); }

	if (type == "AreaEntity")
	{ result = Entity::Ref(new AreaEntity(name, plist)); }

	if (type == "SpawnEntity")
	{ result = Entity::Ref(new SpawnEntity(name, plist)); }

	// set common attributes:

	const CL_Sizef size = readSize(node) / tilesz;
	const CL_Pointf pos = readPos(node) / tilesz;

	result->setPos(pos);
	result->setSize(size);

	return result;
}


//************************************************************************************************************************
