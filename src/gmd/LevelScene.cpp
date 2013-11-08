// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base scene for every game level

#include "LevelScene.h"
#include "LevelFailedScene.h"
#include "LevelDoneScene.h"
#include "LevelPausedScene.h"
#include "Player.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Parsing.h"
#include "../util/BasePath.h"
#include "../util/Generic.h"
#include <assert.h>

//************************************************************************************************************************

static CL_String s_descnames [] = {"level01.desc", "level01.desc", "level01.desc", "level01.desc"};

//************************************************************************************************************************

int LevelScene::countLevels()
{ return COUNTOF(s_descnames); }

GameScene::Ref LevelScene::createLevel(GameManager * manager, int levelNo)
{
	assert(levelNo < countLevels());
	return LevelScene::Ref(new LevelScene(manager, levelNo, s_descnames[levelNo]));
}

//************************************************************************************************************************

LevelScene::LevelScene(GameManager * manager, int levelNo, CL_String descFile)
: GameScene(manager), m_levelNo(levelNo), m_player(CL_Pointf(), CL_Sizef(1.0f, 2.8f))
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// init stuff from desc file:
	loadDescFile(descFile);

	// set initial area:
	enterArea("main", "main");
}

// scene lifecycle:

void LevelScene::update(float secs)
{
	auto ctx = getContext();

	// update player:
	m_player.doUpdate(ctx, secs);

	// update the entities:
	for (auto it = ctx.entities->begin(); it != ctx.entities->end(); /**/)
	{
		auto cur = it ++;
		bool res = (*cur)->doUpdate(ctx, secs);

		// drop the entity, which returned false:
		if (!res) { ctx.entities->erase(cur); }
	}

	// update camera:
	ctx.tilemap->offset(m_player.getRect().get_center());

	// check the globals:
	if (ctx.keys.get_keycode(ctx.cfg->keyPause()))
	{
		m_manager->pushScene(GameScene::Ref(new LevelPausedScene(m_manager)));
	}
	if (m_globals.check(Globals::victory()))
	{
		m_manager->repScene(GameScene::Ref(new LevelDoneScene(m_manager, m_levelNo)));
	}
	if (m_globals.check(Globals::defeat()))
	{
		m_manager->repScene(GameScene::Ref(new LevelFailedScene(m_manager, m_levelNo)));
	}
}

void LevelScene::render()
{
	auto ctx = getContext();
	
	// render map (1st pass):
	ctx.tilemap->renderBackground(ctx);

	// render map (2nd pass):
	ctx.tilemap->renderDecoration(ctx);

	// render player:
	m_player.doRender(ctx);

	// render entities:
	for (auto it = ctx.entities->begin(); it != ctx.entities->end(); ++ it)
	{ (*it)->doRender(ctx); }

	// render map (3rd pass):
	ctx.tilemap->renderForeground(ctx);

	// render HUD:
	m_player.renderHUD(ctx);
}

void LevelScene::notify(Notify code, void * data)
{
	if (code == n_EnterArea)
	{
		auto nadata = (NotifyAreaData*)data;
		enterArea(nadata->area, nadata->entry);
	}
}

// areas management:

void LevelScene::enterArea(CL_String name, CL_String entry)
{
	auto it = m_areas.find(name);
	assert(it != m_areas.end());

	// take entities & tilemap into processing:
	m_entities = it->second.getEntities();
	m_tilemap  = it->second.getTilemap();

	// update player position in the new area:
	m_player.setPos(it->second.getEntryPoint(entry));
	m_player.setVel(CL_Pointf());

	// set correct camera:
	m_tilemap->offset(m_player.getRect().get_center());
}

// level context:

LevelCtx LevelScene::getContext()
{
	Configuration::Ref cf = m_manager->getConfig();
	CL_GraphicContext &gc = m_manager->getRenderer()->getGC();
	CL_InputDevice    &kb = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice    &ms = m_manager->getRenderer()->getIC().get_mouse();

	LevelCtx ctx = { m_manager, cf, gc, kb, ms, m_entities, m_tilemap, m_databags, m_dialogs, m_globals, m_player, m_assets };
	return ctx;
}

// pasing and initialization stuff:

void LevelScene::loadDescFile(CL_String file)
{
	// read sections as databags:
	
	Databags sections; 
	sections.loadBagFile(makePath(file));

	// load all the sections (areas go last):
	
	Databag::Ref databags = sections.find("databags")->second;
	loadDescPart(databags, &LevelScene::loadBagFile);

	Databag::Ref dialogs = sections.find("dialogs")->second;
	loadDescPart(dialogs, &LevelScene::loadDlgFile);

	Databag::Ref resources = sections.find("resources")->second;
	loadDescPart(resources, &LevelScene::loadResource);

	Databag::Ref areas = sections.find("areas")->second;
	loadDescPart(areas, &LevelScene::loadAreaFile);
}

void LevelScene::loadDescPart (Databag::Ref databag, void (LevelScene::*loader)(CL_String, CL_String))
{
	// get all string keys:
	std::list<CL_String> keys; 
	databag->keys<CL_String>(keys);
	
	// process all entries:
	for (auto it = keys.begin(); it != keys.end(); ++ it)
	{ (*this.*loader)(*it, databag->get<CL_String>(*it)); }
}

void LevelScene::loadResource(CL_String name, CL_String file)
{
	m_assets.add_resources(CL_ResourceManager(makePath(file)));
}

void LevelScene::loadAreaFile(CL_String name, CL_String file)
{
	Renderer::Ref renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGC().get_size();

	Area area = Area(window, makePath(file), name);
	m_areas[name] = area;
}

void LevelScene::loadDlgFile(CL_String name, CL_String file)
{
	m_dialogs.loadDlgFile(makePath(file));
}

void LevelScene::loadBagFile(CL_String name, CL_String file)
{
	m_databags.loadBagFile(makePath(file));
}

//************************************************************************************************************************
