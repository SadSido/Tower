// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: shortcuts for creating actual monsters

#ifndef _MonsterTypes_h_
#define _MonsterTypes_h_

#include "../gmd/Entities.h"
#include "../gmd/Databags.h"

//************************************************************************************************************************

// the list of all available monsters:

Entity::Ref createPlant(const Databags &data, const CL_String &name);
Entity::Ref createWalker(const Databags &data, const CL_String &name);
Entity::Ref createGuard(const Databags &data, const CL_String &name);
Entity::Ref createFootman(const Databags &data, const CL_String &name);
Entity::Ref createSniper(const Databags &data, const CL_String &name);

//************************************************************************************************************************

#endif
