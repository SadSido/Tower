// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: map for variant types, useful for quick in-game parameters editing

#include "Databags.h"
#include "../util/Parsing.h"
#include "../util/BasePath.h"
#include <assert.h>

//************************************************************************************************************************

namespace 
{
} // namespace

//************************************************************************************************************************

Databag::Databag()
{
	set("v1", 21);
	set("v2", CL_String("42"));

	int i2 = get<int>("v2");

	// that should be asserted (while it is actually an UB):
	float i3 = get<float>("v3");
}


//************************************************************************************************************************
