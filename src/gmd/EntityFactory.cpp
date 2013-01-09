// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: functions to resolve names and types into actual entities

#include "EntityFactory.h"
#include "EntTest.h"

//************************************************************************************************************************

namespace 
{

template <typename RESULT>
RESULT readAttr(CL_DomElement node, const CL_String &one, const CL_String &two)
{
	return RESULT(node.get_attribute_float(one), node.get_attribute_float(two));
}

CL_Sizef readSize(CL_DomElement node)
{ return readAttr<CL_Sizef>(node, "width", "height"); }

CL_Pointf readPos(CL_DomElement node)
{ return readAttr<CL_Pointf>(node, "x", "y"); }

}

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
