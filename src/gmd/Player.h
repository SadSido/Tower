// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _Player_h_
#define _Player_h_

#include "TileTest.h"
#include "Entity.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class Player : public Entity
{
public:
	explicit Player(CL_Pointf pos, CL_Sizef size);

	virtual bool update(const UpdateCtx &ctx, unsigned int msecs);
	virtual bool render(const RenderCtx &ctx);

private:
	bool      m_ground;
	bool      m_mount;

	float     m_ropeLen;
	CL_Pointf m_rope;
};

//************************************************************************************************************************

#endif
