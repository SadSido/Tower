// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which allows player to pass between areas

#ifndef _AreaEntity_h_
#define _AreaEntity_h_

#include "../gmd/LevelScene.h"
#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class AreaEntity : public Entity
{
public:
	// common constructor for the entities:
	explicit AreaEntity(const CL_DomNodeList &props);

private:
	CL_String m_area;
	CL_String m_entry;

	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);
	virtual void notify(const LevelCtx &ctx, Notify code);
};

//************************************************************************************************************************

#endif
