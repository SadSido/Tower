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
	virtual bool update(const UpdateCtx &ctx, float secs);
	virtual bool render(const RenderCtx &ctx);
	virtual void notify(const UpdateCtx &ctx, Notify code);

private:
	float m_distance;
	DialogSet::Ref m_dlgSet;
};

//************************************************************************************************************************

#endif
