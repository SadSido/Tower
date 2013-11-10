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
};

//************************************************************************************************************************

struct DialogBranch
{
	typedef std::pair<bool, CL_String> Condition;
	typedef std::pair<PhraseType, CL_String> Phrase;

public:
	typedef std::shared_ptr<DialogBranch> Ref;
	typedef std::list<Phrase>::const_iterator Iter;

	explicit DialogBranch();

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


class Dialog
{
public:
	typedef std::shared_ptr<Dialog> Ref;
	explicit Dialog(CL_String::const_iterator &it);

	// get dialog branch for these globals:
	DialogBranch::Ref getBranch(const Globals &globals) const;

private:
	std::list<DialogBranch::Ref> m_branches;
	
	mutable long m_gen;
	mutable DialogBranch::Ref m_dlg;

	// actually choosing dialog here:
	DialogBranch::Ref getBranchImp(const Globals &globals) const;
};

//************************************************************************************************************************

struct Dialogs : public std::map<CL_String, Dialog::Ref>
{
public:
	void loadDlgFile(const CL_String &path);

private:
	void loadDlgFile(CL_String::const_iterator it);
	void loadDialog(CL_String::const_iterator &it);
};

//************************************************************************************************************************

#endif
