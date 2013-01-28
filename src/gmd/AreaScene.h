// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene for switching areas

#ifndef _AreaScene_h_
#define _AreaScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class AreaScene : public GameScene
{
public:
	explicit AreaScene(GameManager * manager);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();
};

//************************************************************************************************************************

#endif
