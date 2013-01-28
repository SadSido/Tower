// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#ifndef _DialogScene_h_
#define _DialogScene_h_

#include "../sys/GameScene.h"
#include "Dialogs.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class DialogScene : public GameScene
{
public:
	explicit DialogScene(GameManager * manager, DialogScript::Ref script, Globals & globals);

	// scene lifecycle:
	virtual void update(float secs);
	virtual void render();

private:
	// input event handlers:
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

private:
	GameScene::Ref m_topScene;
	CL_SlotContainer m_slots;

	CL_Font_System m_font;
	CL_SpanLayout  m_layout;
	
	CL_Rect m_rcBub;
	CL_Rect m_rcText;

	float m_percent;
	PhraseType m_oldtype;
	PhraseType m_newtype;

	DialogScript::Ref m_script;
	DialogScript::Iter m_iter;
	Globals & m_globals;

	void updateRects(CL_Size window);
	void updateLayout();
};

//************************************************************************************************************************

#endif
