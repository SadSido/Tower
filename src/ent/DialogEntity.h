// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which triggers a dialog scene

#ifndef _DialogEntity_h_
#define _DialogEntity_h_

#include "../gmd/LevelScene.h"
#include "../gmd/Entities.h"
#include "../gmd/Dialogs.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class DialogAction : public PlayerAction
{
public:
	explicit DialogAction(DialogScript::Ref script);
	virtual void execute(const UpdateCtx &ctx);

private:
	DialogScript::Ref m_script;
};

//************************************************************************************************************************

class DialogEntity : public Entity
{
public:
	// common constructor for the entities:
	explicit DialogEntity(CL_String name, const CL_DomNodeList &props);

	virtual bool update(const UpdateCtx &ctx, float secs);
	virtual bool render(const RenderCtx &ctx);

	// assign / unassign action to player:
	void assignAction(Player& player, DialogScript::Ref script);
	void resetAction(Player& player);

private:
	bool m_assigned;
	float m_distance;
	DialogSet::Ref m_dlgSet;
};

//************************************************************************************************************************

#endif
