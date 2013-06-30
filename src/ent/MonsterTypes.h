// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: shortcuts for creating actual monsters

#ifndef _MonsterTypes_h_
#define _MonsterTypes_h_

#include "../gmd/Entities.h"

//************************************************************************************************************************

// the list of all available monsters:

Entity::Ref createPlant(const CL_DomNodeList &props);
Entity::Ref createWalker(const CL_DomNodeList &props);

//************************************************************************************************************************

#endif
