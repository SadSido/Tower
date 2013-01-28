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
	typedef std::list<Phrase>::const_iterator Iter;

	explicit DialogScript();

	// handling precs and posts:
	bool checkPrecs(const Globals &globals) const;
	void applyPosts(Globals &globals) const;

	// script editing:
	void addPrec (bool direct, const CL_String &global);
	void addPost (bool direct, const CL_String &global);
	void addText (PhraseType type, const CL_String &text);

	// iterating through the script:
	Iter begin() const { return m_texts.begin(); }
	Iter end() const { return m_texts.end(); }

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

	// get dialog for these globals:
	DialogScript::Ref getDialog(const Globals &globals) const;

private:
	std::list<DialogScript::Ref> m_dialogs;
	
	void loadDlgFile(CL_String::const_iterator it);
	void loadDialog(CL_String::const_iterator it);

	mutable long m_gen;
	mutable DialogScript::Ref m_dlg;

	// actually choosing dialog here:
	DialogScript::Ref getDialogImp(const Globals &globals) const;
};

//************************************************************************************************************************

struct Dialogs : public std::map<CL_String, DialogSet::Ref>
{};

//************************************************************************************************************************

#endif
