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
	enum PosFlags
	{
		pf_OnGround  = 1 << 0,
		pf_OnStairs  = 1 << 1,
		pf_TopStairs = 1 << 2,
	};

	// action state flags:
	bool m_climbing;

	// tilemap helpers:
	int getPosFlags(const UpdateCtx &ctx);

	// input helpers:
	void handleUpKey(const UpdateCtx &ctx, int posFlags);
	void handleDownKey(const UpdateCtx &ctx, int posFlags);
	void handleLeftKey(const UpdateCtx &ctx, int posFlags);
	void handleRightKey(const UpdateCtx &ctx, int posFlags);
};

//************************************************************************************************************************

#endif
