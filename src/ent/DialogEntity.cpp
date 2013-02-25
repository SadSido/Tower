// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which triggers a dialog scene

#include "DialogEntity.h"
#include "../sys/GameManager.h"
#include "../gmd/DialogScene.h"
#include <assert.h>

//************************************************************************************************************************
	
DialogEntity::DialogEntity(CL_String name, const CL_DomNodeList &props)
: Entity("DialogEntity", name)
{
}

bool DialogEntity::update(const LevelCtx &ctx, float secs, int msecs)
{
	// checks whether we have a dialog available:
	if (!m_dlgSet) { m_dlgSet = ctx.dialogs[m_name]; }

	auto dialog = m_dlgSet->getDialog(ctx.globals);
	
	const bool isInside = m_rect.is_inside(ctx.player.getRect());
	const bool assigned = ctx.player.checkAction(this);

	if (!assigned && dialog && isInside)
	{ ctx.player.setAction(this); }

	else if (assigned && (!dialog || !isInside))
	{ ctx.player.setAction(NULL); }

	// perform the rest of the update:
	// ...
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
	if (auto dialog = m_dlgSet->getDialog(ctx.globals))
	{
		GameScene::Ref dlgScene(new DialogScene(ctx.manager, dialog, ctx.globals));
		ctx.manager->pushScene(dlgScene);
	}
}

//************************************************************************************************************************
