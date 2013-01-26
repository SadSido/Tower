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

class PlayerAction
{
public:
	typedef std::shared_ptr<PlayerAction> Ref;
	virtual ~PlayerAction() {}

	// action to be triggered by action key:
	virtual void execute(const UpdateCtx &ctx) = 0;
};

//************************************************************************************************************************

class Player : public Entity
{
public:
	explicit Player(CL_Pointf pos, CL_Sizef size);

	virtual bool update(const UpdateCtx &ctx, float secs);
	virtual bool render(const RenderCtx &ctx);

	// assign an action to the player:
	void setAction(PlayerAction::Ref action)
	{ m_action = action; }

private:
	enum PosFlags
	{
		pf_OnGround  = 1 << 0,
		pf_OnStairs  = 1 << 1,
		pf_TopStairs = 1 << 2,
	};

	// action state flags:
	bool m_climbing;

	// current action if any:
	PlayerAction::Ref m_action;

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
