// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for providing access to game config

#ifndef _Configuration_h_
#define _Configuration_h_

#include <memory>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class Configuration
{
public:
	typedef std::shared_ptr<Configuration> Ref;
	explicit Configuration();

	// assigned controls:

	int keyUp      () const;
	int keyDown    () const;
	int keyLeft    () const;
	int keyRight   () const;
	int keyPause   () const;
	int mouseLeft  () const;
	int mouseRight () const;
};

//************************************************************************************************************************

#endif
