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

	// interaction rects:
	CL_Rectf getSwordRect();
	CL_Rectf getShieldRect();

private:
	// current action if any:
	Entity * m_action;

	// tilemap helpers:
	int getPosFlags(const LevelCtx &ctx);

	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

	// per-state updates:
	void enterAction   (const LevelCtx &ctx);
	void enterState    (int state, CL_Pointf vel, CL_Pointf acc);

	void update_Stand  (const LevelCtx &ctx, int posFlags);
	void update_Walk   (const LevelCtx &ctx, int posFlags);
	void update_Climb  (const LevelCtx &ctx, int posFlags);
	void update_Jump   (const LevelCtx &ctx, int posFlags);
	void update_Shield (const LevelCtx &ctx, int posFlags);
	void update_Pierce (const LevelCtx &ctx, int posFlags);
	void update_Slash  (const LevelCtx &ctx, int posFlags);
	void update_Strike (const LevelCtx &ctx, int posFlags);

	// rectangle helpers:
	CL_Rectf getHitmapRect();
};

//************************************************************************************************************************

#endif
