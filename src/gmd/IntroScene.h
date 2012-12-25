// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#ifndef _IntroScene_h_
#define _IntroScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class IntroScene : public GameScene
{
public:
	explicit IntroScene(GameManager * manager);

	virtual void update(float secs);
	virtual void render();

private:
	CL_Sprite m_logo;
};

//************************************************************************************************************************

#endif
