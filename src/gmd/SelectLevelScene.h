// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene for choosing starting level

#ifndef _SelectLevelScene_h_
#define _SelectLevelScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class SelectLevelScene : public GameScene
{
public:
	// c-tors and d-tors:
	explicit SelectLevelScene(GameManager * manager);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
	// input event handlers:
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

private:
	CL_SlotContainer m_slots;
	CL_Font_System m_font;
	
	// selected item:
	int m_selected;
};

//************************************************************************************************************************

#endif
