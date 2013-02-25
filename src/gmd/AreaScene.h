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
	explicit AreaScene(GameManager * manager, CL_String areaName, CL_String entryName);

	// scene lifecycle:
	virtual void update(float secs, int msecs);
	virtual void render();

private:
	bool  m_direct;
	float m_percent;

	CL_String m_areaName;
	CL_String m_entryName;

	GameScene::Ref m_topScene;
};

//************************************************************************************************************************

#endif
