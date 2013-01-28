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
	explicit AreaEntity(CL_String name, const CL_DomNodeList &props);

	// virtual entity interface:
	virtual bool update(const UpdateCtx &ctx, float secs);
	virtual bool render(const RenderCtx &ctx);
	virtual void notify(const UpdateCtx &ctx, Notify code);

private:
	float m_distance;

	CL_String m_area;
	CL_String m_entry;
};

//************************************************************************************************************************

#endif
