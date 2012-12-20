// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _EntTest_h_
#define _EntTest_h_

#include "TileTest.h"
#include "Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class EntTest : public Entity
{
public:
	explicit EntTest(CL_Pointf pos, CL_Sizef size);

	virtual bool update(const UpdateCtx &ctx, unsigned int msecs);
	virtual bool render(const RenderCtx &ctx);

private:
	bool m_collapse;
};

//************************************************************************************************************************

#endif
