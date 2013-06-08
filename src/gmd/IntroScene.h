// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, playing intro before the level

#ifndef _IntroScene_h_
#define _IntroScene_h_

#include "../sys/GameScene.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class IntroScene : public GameScene
{
public:
	// factory for creating intros:
	static GameScene::Ref createIntro(GameManager * manager, int levelNo);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
	// input event handlers:
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

private:
	// intro scene number:
	const int m_levelNo;

	float m_offset;
	CL_SlotContainer m_slots;
	CL_Font_System m_font;

	// constructor:
	explicit IntroScene(GameManager * manager, int levelNo);

	// scene helpers:
	void proceedToLevel();
};

//************************************************************************************************************************

#endif
