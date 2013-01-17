// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes, required to run dialog scripts

#ifndef _Dialogs_h_
#define _Dialogs_h_

#include "Globals.h"
#include <ClanLib/core.h>
#include <map>
#include <list>

//************************************************************************************************************************

enum PhraseType
{
	pht_Player,
	pht_NPC,
	pht_None,
};

//************************************************************************************************************************

struct DialogScript
{
	typedef std::pair<bool, CL_String> Condition;
	typedef std::pair<PhraseType, CL_String> Phrase;

public:
	typedef std::shared_ptr<DialogScript> Ref;
	explicit DialogScript();

	// script editing:
	void addPrec (bool direct, const CL_String &global);
	void addPost (bool direct, const CL_String &global);
	void addText (PhraseType type, const CL_String &text);

private:
	std::list<Condition> m_precs;
	std::list<Condition> m_posts;
	std::list<Phrase>    m_texts;
};


class DialogSet
{
public:
	typedef std::shared_ptr<DialogSet> Ref;
	explicit DialogSet(CL_String path);

	// select current dialog:
	DialogScript::Ref getDialog(const Globals &globals) const;

private:
	std::list<DialogScript::Ref> m_dialogs;
	void loadDlgFile(CL_String::const_iterator it);
};

//************************************************************************************************************************

struct Dialogs : public std::map<CL_String, DialogSet::Ref>
{};

//************************************************************************************************************************

#endif
