// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene for choosing starting level

#ifndef _LevelSelectScene_h_
#define _LevelSelectScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class LevelSelectScene : public GameScene
{
public:
	explicit LevelSelectScene(GameManager * manager);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
};

//************************************************************************************************************************

#endif
