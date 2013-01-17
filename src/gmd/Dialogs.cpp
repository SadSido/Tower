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
		parseAssert(it, "{");		
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
		parseAssert(it, "{");		
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
{
	CL_String mkpath = makePath(path);
	CL_String source = CL_File::read_text(mkpath);

	// init stuff from dlg file:
	loadDlgFile(source.begin());
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

//************************************************************************************************************************
