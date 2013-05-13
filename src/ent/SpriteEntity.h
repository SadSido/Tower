// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity which is merely playing a sprite animation

#ifndef _SpriteEntity_h_
#define _SpriteEntity_h_

#include "../gmd/LevelScene.h"
#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class SpriteEntity : public Entity
{
public:
	// common constructor for the entities:
	explicit SpriteEntity(const CL_DomNodeList &props);

private:
	// virtual entity interface:
	virtual bool update(const LevelCtx &ctx, float secs);
	virtual bool render(const LevelCtx &ctx);
	virtual void upload(const LevelCtx &ctx);

private:
	// properties:
	CL_String m_sprName;
	CL_Sprite m_sprite;
};

//************************************************************************************************************************

#endif
