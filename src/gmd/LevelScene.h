// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base scene for every game level

#ifndef _LevelScene_h_
#define _LevelScene_h_

#include "../sys/GameScene.h"
#include "../sys/Configuration.h"
#include "Areas.h"
#include "Tilemap.h"
#include "Globals.h"
#include "Dialogs.h"
#include "Player.h"
#include "Databags.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

// more laconic name for resource manager:
typedef CL_ResourceManager Assets;

// level objects get this when they calculate update:
struct LevelCtx
{
	GameManager * manager;
	Configuration::Ref cfg;

	CL_GraphicContext &gc;
	CL_InputDevice &keys;
	CL_InputDevice &mouse;

	Entities::Ref entities;
	Tilemap::Ref  tilemap;

	Databags &databags;
	Dialogs  &dialogs;
	Globals  &globals;
	Player   &player;
	Assets   &assets;
};

// data, passed to "switch areas" notification:

struct NotifyAreaData
{
	CL_String area;
	CL_String entry;
};

//************************************************************************************************************************

class LevelScene : public GameScene
{
public:
	// factory for creating levels:
	static int countLevels();
	static GameScene::Ref createLevel(GameManager * manager, int levelNo);

	// scene lifecycle:	
	virtual void update(float secs);
	virtual void render();
	virtual void notify(Notify code, void * data);

	// area management:
	void enterArea(CL_String name, CL_String entry);

private:
	const int m_levelNo;

	Entities::Ref m_entities;
	Tilemap::Ref  m_tilemap;
	
	Databags  m_databags;
	Dialogs   m_dialogs;
	Globals   m_globals;
	Player    m_player;
	Areas     m_areas;
	Assets    m_assets;

	// constructor:
	explicit LevelScene(GameManager * manager, int levelNo, CL_String descFile);

	// generating context:
	LevelCtx getContext();

	// parsing the desc file:
	void loadDescFile(CL_String::const_iterator  it);
	void loadResource(CL_String::const_iterator &it);
	void loadAreaFile(CL_String::const_iterator &it);
	void loadDlgFile(CL_String::const_iterator &it);
	void loadBagFile(CL_String::const_iterator &it);
};

//************************************************************************************************************************

#endif
