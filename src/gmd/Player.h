// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _Player_h_
#define _Player_h_

#include "Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

struct UpdateCtx;
struct RenderCtx;

//************************************************************************************************************************

class Player : public Entity
{
public:
	explicit Player(CL_Pointf pos, CL_Sizef size);

	virtual bool update(const UpdateCtx &ctx, float secs);
	virtual bool render(const RenderCtx &ctx);

private:
	bool      m_ground;
	bool      m_mount;
	bool      m_stairs;

	float     m_ropeLen;
	CL_Pointf m_rope;

	bool standOnGround(const UpdateCtx &ctx);
	bool standOnStairs(const UpdateCtx &ctx);
};

//************************************************************************************************************************

#endif
