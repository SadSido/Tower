// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _EntTest_h_
#define _EntTest_h_

#include "LevelScene.h"
#include "Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class EntTest : public Entity
{
public:
	explicit EntTest(CL_Pointf pos, CL_Sizef size);

	virtual bool update(const UpdateCtx &ctx, float secs);
	virtual bool render(const RenderCtx &ctx);

private:
	bool m_collapse;
};

//************************************************************************************************************************

#endif
