// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes, required to run dialog scripts

#include "Dialogs.h"
#include "../util/Parsing.h"
#include "../util/BasePath.h"

//************************************************************************************************************************

namespace 
{
	PhraseType getPhraseType(const CL_String &desc)
	{
		if (desc == "play") { return pht_Player; }
		if (desc == "npc")  { return pht_NPC;    }
		if (desc == "none") { return pht_None;   }

		// assert(false);
		return pht_None;
	}

	// parsing conditions:

	template<void (DialogScript::*FUNC)(bool, const CL_String&)>
	void parseConditions(CL_String::const_iterator &it, DialogScript::Ref script)
	{
		// allow empty braces:
		CL_String start = parseToken(it);
		if (start == "{}") return;

		// assert(it == "{");		
		for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
		{
			// direct or reverse condition?
			const bool direct = (token[0] != '~');
			const int  starts = (direct) ? 0 : 1;

			// apply parsed condition:
			(*script.*FUNC)(direct, token.substr(starts));
		}
	}

	void parsePrecs(CL_String::const_iterator &it, DialogScript::Ref script)
	{ parseConditions<&DialogScript::addPrec>(it, script);	}

	void parsePosts(CL_String::const_iterator &it, DialogScript::Ref script)
	{ parseConditions<&DialogScript::addPost>(it, script);	}

	// parsing scenario:

	void parsePhrases(CL_String::const_iterator &it, DialogScript::Ref script)
	{
		// allow empty braces:
		CL_String start = parseToken(it);
		if (start == "{}") return;

		// assert(it == "{");		
		for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
		{
			// parsing <type "phrase"> pattern:
			const PhraseType type = getPhraseType(token);
			const CL_String  text = parseQuotes(it);

			// apply the phrase:
			script->addText(type, text);
		}
	}

} // namespace

//************************************************************************************************************************

DialogScript::DialogScript()
{
}

bool DialogScript::checkPrecs(const Globals &globals) const
{
	for (auto it = m_precs.begin(); it != m_precs.end(); ++ it)
	{
		// xor is the operation for this!
		if (it->first ^ globals.check(it->second)) return false;
	}
	return true;
}

void DialogScript::applyPosts(Globals &globals) const
{
	for (auto it = m_posts.begin(); it != m_posts.end(); ++ it)
	{
		(it->first) ? globals.add(it->second) 
			        : globals.del(it->second);
	}
}

void DialogScript::addPrec(bool direct, const CL_String &global)
{
	m_precs.push_back(std::make_pair(direct, global));
}

void DialogScript::addPost(bool direct, const CL_String &global)
{
	m_posts.push_back(std::make_pair(direct, global));
}

void DialogScript::addText(PhraseType type, const CL_String &text)
{
	m_texts.push_back(std::make_pair(type, text));
}

//************************************************************************************************************************

DialogSet::DialogSet(CL_String path)
: m_gen(0)
{
	CL_String mkpath = makePath(path);
	CL_String source = CL_File::read_text(mkpath);

	// init stuff from dlg file:
	loadDlgFile(source.begin());
}

DialogScript::Ref DialogSet::getDialog(const Globals &globals) const
{
	// maybe precalculated already:
	if (m_gen == globals.getGen())
	{ return m_dlg; } 

	// must calculate for these globals:
	m_dlg = getDialogImp(globals);
	m_gen = globals.getGen();

	// may be null here:
	return m_dlg;
}

void DialogSet::loadDlgFile(CL_String::const_iterator it)
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
		{ /* assert(false); */ }
	}
}

void DialogSet::loadDialog(CL_String::const_iterator it)
{
	DialogScript::Ref script(new DialogScript());

	// read dialog parts:
	parsePrecs(it, script);
	parsePosts(it, script);
	parsePhrases(it, script);

	// commit to the list:
	m_dialogs.push_back(script);
}

DialogScript::Ref DialogSet::getDialogImp(const Globals &globals) const
{
	for (auto it = m_dialogs.begin(); it != m_dialogs.end(); ++ it)
	{ if ((*it)->checkPrecs(globals)) return (*it); }

	// none of the dialogs satisfies:
	return DialogScript::Ref(); 
}

//************************************************************************************************************************
