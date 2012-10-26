// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: application entry point

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include "sys/GameManager.h"

//************************************************************************************************************************

int main(const std::vector<CL_String> &args)
{
	return GameManager().runMainLoop();
}

// cross-platform entry point:
CL_ClanApplication app(main);

//************************************************************************************************************************

