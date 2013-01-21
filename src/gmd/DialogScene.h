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
	explicit DialogScene(GameManager * manager, DialogScript::Ref script);

	virtual void update(float secs);
	virtual void render();

private:
	GameScene::Ref m_topScene;
	DialogScript::Ref m_script;
};

//************************************************************************************************************************

#endif