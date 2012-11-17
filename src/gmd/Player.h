// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _Player_h_
#define _Player_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

class Player
{
public:
	explicit Player();

	void update(unsigned int msecs);
	void render();
};

//************************************************************************************************************************

#endif
