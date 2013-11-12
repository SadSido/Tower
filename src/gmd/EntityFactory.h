// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: functions to resolve names and types into actual entities

#ifndef _EntityFactory_h_
#define _EntityFactory_h_

#include <ClanLib/core.h>
#include "Entities.h"
#include "Databags.h"

//************************************************************************************************************************

Entity::Ref createEntity (const Databags &data, const CL_DomElement &node, float tilesz);
Entity::Ref createEntity (const Databags &data, const CL_String &type, const CL_String &name);

//************************************************************************************************************************

#endif
