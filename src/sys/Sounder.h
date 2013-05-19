// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class, incapsulating sound and music playback

#ifndef _Sounder_h_
#define _Sounder_h_

#include <memory>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class Sounder
{
public:
	typedef std::shared_ptr<Sounder> Ref;
	explicit Sounder();
};

//************************************************************************************************************************

#endif
