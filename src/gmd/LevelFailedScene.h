// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, that allows player to restart the level

#ifndef _LevelFailedScene_h_
#define _LevelFailedScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class LevelFailedScene : public GameScene
{
public:
	// c-tors and d-tors:
	explicit LevelFailedScene(GameManager * manager, int levelNo);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
	// input event handlers:
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

private:
	CL_SlotContainer m_slots;
	CL_Font_System m_font;
	
	// level to restart:
	const int m_levelNo;
};

//************************************************************************************************************************

#endif
