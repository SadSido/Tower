// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: application entry point

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include "sys/GameManager.h"

//************************************************************************************************************************

enum ErrorCodes
{ ec_Success, ec_Failure };

//************************************************************************************************************************

int main(const std::vector<CL_String> &args)
{
	try 
	{
		GameManager().runMainLoop();
		return ec_Success;
	}
	catch (CL_Exception ex)
	{
		auto desc = ex.what();
		return ec_Failure;
	}
}

// cross-platform entry point:
CL_ClanApplication app(main);

//************************************************************************************************************************

