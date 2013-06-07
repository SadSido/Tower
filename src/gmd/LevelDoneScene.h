// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, that allows player to proceed to the next level

#ifndef _LevelDoneScene_h_
#define _LevelDoneScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class LevelDoneScene : public GameScene
{
public:
	// c-tors and d-tors:
	explicit LevelDoneScene(GameManager * manager, int levelNo);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
	// input event handlers:
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

private:
	CL_SlotContainer m_slots;
	CL_Font_System m_font;
	
	// level, which was completed:
	const int m_levelNo;
};

//************************************************************************************************************************

#endif
