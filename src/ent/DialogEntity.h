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
	explicit DialogEntity(CL_String name, const CL_DomNodeList &props);

	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs, int msecs);
	virtual bool render(const LevelCtx &ctx);
	virtual void notify(const LevelCtx &ctx, Notify code);

private:
	DialogSet::Ref m_dlgSet;
};

//************************************************************************************************************************

#endif
