// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which triggers a dialog scene

#include "DialogEntity.h"
#include "../sys/GameManager.h"
#include "../gmd/DialogScene.h"
#include <assert.h>

//************************************************************************************************************************
	
DialogEntity::DialogEntity(const Databags &data, const CL_String &name)
{
	auto bag = data.find(name)->second;
	m_dlgName = bag->get<CL_String>("dialog");
}

bool DialogEntity::update(const LevelCtx &ctx, float secs)
{
	// checks whether we have a dialog available:
	if (!m_dialog) { m_dialog = ctx.dialogs[m_dlgName]; }

	auto branch = m_dialog->getBranch(ctx.globals);
	
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && branch && isInside)
	{ ctx.player.setAction(this); }

	else if (assigned && (!branch || !isInside))
	{ ctx.player.setAction(NULL); }

	return true;
}

bool DialogEntity::render(const LevelCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,255));
	return true;
}


void DialogEntity::notify(const LevelCtx &ctx, Notify code)
{
	assert(code == n_DoAction);
	if (auto branch = m_dialog->getBranch(ctx.globals))
	{
		GameScene::Ref dlgScene(new DialogScene(ctx.manager, branch, ctx.globals));
		ctx.manager->pushScene(dlgScene);
	}
}

//************************************************************************************************************************
