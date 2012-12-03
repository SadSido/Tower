// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes, required to run dialog scripts

#include "Dialogs.h"
#include "../util/Parsing.h"

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

	template<void (DlgScript::*FUNC)(bool, const CL_String&)>
	void parseConditions(CL_String::iterator &it, DlgScript &script)
	{
		parseAssert(it, "{");		
		for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
		{
			// direct or reverse condition?
			const bool direct = (token[0] != '~');
			const int  starts = (direct) ? 0 : 1;

			// apply parsed condition:
			(script.*FUNC)(direct, token.substr(starts));
		}
	}

	void parsePrecs(CL_String::iterator &it, DlgScript &script)
	{
		parseConditions<&DlgScript::addPrec>(it, script);
	}

	void parsePosts(CL_String::iterator &it, DlgScript &script)
	{
		parseConditions<&DlgScript::addPost>(it, script);
	}

	// parsing scenario:

	void parsePhrases(CL_String::iterator &it, DlgScript &script)
	{
		parseAssert(it, "{");		
		for (CL_String token = parseToken(it); token != "}"; token = parseToken(it))
		{
			// parsing <type "phrase"> pattern:
			const PhraseType type = getPhraseType(token);
			const CL_String  text = parseQuotes(it);

			// apply the phrase:
			script.addText(type, text);
		}
	}

	// parsing entire script:

	void parseDialog(CL_String::iterator &it, DlgScript &script)
	{
		parseAssert(it, "dialog");
		CL_String owner  = parseToken(it);

		parsePrecs(it, script);
		parsePosts(it, script);
		parsePhrases(it, script);
	}

}

//************************************************************************************************************************

DlgScript::DlgScript()
{
}

void DlgScript::addPrec(bool direct, const CL_String &global)
{
	m_precs.push_back(std::make_pair(direct, global));
}

void DlgScript::addPost(bool direct, const CL_String &global)
{
	m_posts.push_back(std::make_pair(direct, global));
}

void DlgScript::addText(PhraseType type, const CL_String &text)
{
	m_texts.push_back(std::make_pair(type, text));
}

//************************************************************************************************************************
