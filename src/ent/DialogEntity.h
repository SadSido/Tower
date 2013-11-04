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

class DialogEntity : public Entity
{
public:
	// common constructor for the entities:
	explicit DialogEntity(const CL_DomNodeList &props);

private:
	CL_String m_dlgName;
	Dialog::Ref m_dialog;

	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);
	virtual void notify(const LevelCtx &ctx, Notify code);
};

//************************************************************************************************************************

#endif
