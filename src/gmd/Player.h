// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _Player_h_
#define _Player_h_

#include "Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

struct LevelCtx;

//************************************************************************************************************************

class Player : public Entity
{
public:
	explicit Player(CL_Pointf pos, CL_Sizef size);

	// assign an action to the player:
	void setAction(Entity * action);
	bool checkAction(Entity * action);

private:
	#pragma region enums

	enum PosFlags
	{
		pf_OnGround  = 1 << 0,
		pf_OnStairs  = 1 << 1,
		pf_TopStairs = 1 << 2,
	};
	enum Sprites
	{
		spr_Stand,
		spr_Walk,
		spr_Climb,
		spr_Jump,
		spr_Count
	};
	
	#pragma endregion

private:
	// current action if any:
	Entity * m_action;

	// tilemap helpers:
	int getPosFlags(const LevelCtx &ctx);

	// sprites & rendering:
	float m_facing;

	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs, int msecs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

	// per-state updates:
	void update_Stand (const LevelCtx &ctx, int posFlags);
	void update_Walk  (const LevelCtx &ctx, int posFlags);
	void update_Climb (const LevelCtx &ctx, int posFlags);
	void update_Jump  (const LevelCtx &ctx, int posFlags);
};

//************************************************************************************************************************

#endif
