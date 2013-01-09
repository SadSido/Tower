// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: functions to resolve names and types into actual entities

#include "EntityFactory.h"
#include "../util/XmlUtils.h"
#include "EntTest.h"

//************************************************************************************************************************

Entity::Ref createEntity(CL_DomElement node, float tilesz)
{
	const CL_String name = node.get_attribute("name");
	const CL_String type = node.get_attribute("type");

	Entity::Ref result;
	
	// generate entity by type:

	if (type == "EntTest")
	{ result = Entity::Ref(new EntTest(name, node)); }

	// set common attributes:

	const CL_Sizef size = readSize(node) / tilesz;
	const CL_Pointf pos = readPos(node) / tilesz;

	result->setPos(pos);
	result->setSize(size);

	return result;
}


//************************************************************************************************************************
