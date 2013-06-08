// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, that allows player to quit / pause the level

#ifndef _LevelPausedScene_h_
#define _LevelPausedScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class LevelPausedScene : public GameScene
{
public:
	// c-tors and d-tors:
	explicit LevelPausedScene(GameManager * manager);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
	// input event handlers:
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

private:
	CL_SlotContainer m_slots;
	CL_Font_System m_font;

	// scene for sub-rendering:
	GameScene::Ref m_topScene;

	// animation parameters:
	float m_percent;
};

//************************************************************************************************************************

#endif
