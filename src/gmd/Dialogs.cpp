// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes, required to run dialog scripts

#include "Dialogs.h"
#include "../util/Parsing.h"
#include "../util/BasePath.h"
#include <assert.h>

//************************************************************************************************************************

namespace 
{
	PhraseType getPhraseType(const CL_String &desc)
	{
		if (desc == "<") { return pht_Player; }
		if (desc == ">") { return pht_NPC;    }

		assert(false);
		return pht_NPC;
	}

	// parsing conditions:

	template<void (DialogBranch::*FUNC)(bool, const CL_String&)>
	void parseConditions(CL_String::const_iterator &it, DialogBranch::Ref branch)
	{
		// allow empty braces:
		CL_String start = parseToken(it);
		if (start == "{}") return;

		assert(start == "{");		
		for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
		{
			// direct or reverse condition?
			const bool direct = (token[0] != '~');
			const int  starts = (direct) ? 0 : 1;

			// apply parsed condition:
			(*branch.*FUNC)(direct, token.substr(starts));
		}
	}

	void parsePrecs(CL_String::const_iterator &it, DialogBranch::Ref branch)
	{ parseConditions<&DialogBranch::addPrec>(it, branch);	}

	void parsePosts(CL_String::const_iterator &it, DialogBranch::Ref branch)
	{ parseConditions<&DialogBranch::addPost>(it, branch);	}

	// parsing scenario:

	void parsePhrases(CL_String::const_iterator &it, DialogBranch::Ref branch)
	{
		// allow empty braces:
		CL_String start = parseToken(it);
		if (start == "{}") return;

		assert(start == "{");		
		for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
		{
			// parsing <type "phrase"> pattern:
			const PhraseType type = getPhraseType(token);
			const CL_String  text = parseQuotes(it);

			// apply the phrase:
			branch->addText(type, text);
		}
	}

} // namespace

//************************************************************************************************************************

DialogBranch::DialogBranch()
{
}

bool DialogBranch::checkPrecs(const Globals &globals) const
{
	for (auto it = m_precs.begin(); it != m_precs.end(); ++ it)
	{
		// xor is the operation for this!
		if (it->first ^ globals.check(it->second)) return false;
	}
	return true;
}

void DialogBranch::applyPosts(Globals &globals) const
{
	for (auto it = m_posts.begin(); it != m_posts.end(); ++ it)
	{
		(it->first) ? globals.add(it->second) 
			        : globals.del(it->second);
	}
}

void DialogBranch::addPrec(bool direct, const CL_String &global)
{
	m_precs.push_back(std::make_pair(direct, global));
}

void DialogBranch::addPost(bool direct, const CL_String &global)
{
	m_posts.push_back(std::make_pair(direct, global));
}

void DialogBranch::addText(PhraseType type, const CL_String &text)
{
	m_texts.push_back(std::make_pair(type, text));
}

//************************************************************************************************************************

Dialog::Dialog(CL_String::const_iterator &it)
: m_gen(0)
{
	// allow empty braces:
	CL_String start = parseToken(it);
	if (start == "{}") return;

	assert(start == "{");		
	for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
	{
		assert(token == "branch");
		DialogBranch::Ref branch(new DialogBranch());

		// read branch parts:
		parsePrecs(it, branch);
		parsePosts(it, branch);
		parsePhrases(it, branch);

		// commit to the list:
		m_branches.push_back(branch);
	}
}

DialogBranch::Ref Dialog::getBranch(const Globals &globals) const
{
	// maybe precalculated already:
	if (m_gen == globals.getGen())
	{ return m_dlg; } 

	// must calculate for these globals:
	m_dlg = getBranchImp(globals);
	m_gen = globals.getGen();

	// may be null here:
	return m_dlg;
}

DialogBranch::Ref Dialog::getBranchImp(const Globals &globals) const
{
	for (auto it = m_branches.begin(); it != m_branches.end(); ++ it)
	{ if ((*it)->checkPrecs(globals)) return (*it); }

	// none of the dialogs satisfies:
	return DialogBranch::Ref(); 
}

//************************************************************************************************************************

void Dialogs::loadDlgFile(CL_String path)
{
	CL_String source = CL_File::read_text(path);
	loadDlgFile(source.begin());
}

void Dialogs::loadDlgFile(CL_String::const_iterator it)
{
	while (*it)
	{
		CL_String token = parseToken(it);

		// allow single-line comments:
		if (token == "//")
		{ parseLine(it); }
			
		else if (token == "dialog")
		{ loadDialog(it); }

		else
		{ assert(token.empty()); }
	}
}

void Dialogs::loadDialog(CL_String::const_iterator &it)
{
	// parse out name and dialog:
	auto name = parseToken(it);
	auto dial = Dialog::Ref(new Dialog(it));

	// add dialog to the registry:
	operator[](name) = dial;
}

//************************************************************************************************************************
