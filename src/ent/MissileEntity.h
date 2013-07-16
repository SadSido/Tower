// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for the shootable projectiles

#ifndef _MissileEntity_h_
#define _MissileEntity_h_

#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <memory>

//************************************************************************************************************************

struct LevelCtx;
struct TileTest;

//************************************************************************************************************************

class MissileEntity : public Entity
{
public:
	// c-tors and d-tors:
	explicit MissileEntity(const CL_DomNodeList &props);
	Entity::Ref clone();

private:
	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);
	virtual void notify (const LevelCtx &ctx, Notify code);

private:
	int m_bounces;
	CL_String m_sprite;
};

//************************************************************************************************************************

#endif
