// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes, required to run dialog scripts

#ifndef _Dialogs_h_
#define _Dialogs_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

class DlgScript
{
public:
	explicit DlgScript(CL_String owner);

private:
	CL_String m_owner;
};

//************************************************************************************************************************

#endif
