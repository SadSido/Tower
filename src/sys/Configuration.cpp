// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for providing access to game config

#include "Configuration.h"

//************************************************************************************************************************

Configuration::Configuration()
{
	// resolving base path for the game:
	m_basePath = CL_File::read_text("basepath");
}

//************************************************************************************************************************

