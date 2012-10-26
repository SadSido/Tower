// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for providing access to game config

#ifndef _Configuration_h_
#define _Configuration_h_

#include <memory>
#include <ClanLib/core.h>

//************************************************************************************************************************

class Configuration
{
public:
	typedef std::shared_ptr<Configuration> Ref;
	explicit Configuration();

	CL_String getPath(CL_String fname) const
	{ return m_basePath + fname; }

private:
	CL_String m_basePath;
};

//************************************************************************************************************************

#endif
