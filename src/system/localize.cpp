/******************************************************************************/
/*!
\file	localize.cpp
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Localization functionality
\copyright    All content ?2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "localize.h"
#include "../messaging/messagehandler.h"
#include "../messaging/messaging.h"
#include "../messaging/InputEvent.h"
#include "common_defines.h"
#include "TrueType.h"
#include "../utils/utils.h"

namespace System
{
	LocaleHandler * lochandler = nullptr;

	static constexpr std::string_view const defaultLocale = "en_US.UTF-8";
	static std::string currentLocale = "en_US.UTF-8";

	void InitLocalization()
	{
		lochandler = new LocaleHandler();
	}

	void ShutdownLocalization()
	{
		delete lochandler;
		lochandler = nullptr;
	}

	void SetLocale(std::string path, std::string domain, std::string lstring)
	{
		lochandler->SetLocale(path, domain, lstring);
	}

	std::string GetLocale()
	{
		return lochandler->GetLocale();
	}
}

LocaleHandler::LocaleHandler()
{
	Messaging::messHandler->Attach<LocaleHandler, InputEvent, &LocaleHandler::InputHandler>(this, "InputEvent");

	gen.add_messages_path("./localization");
	gen.add_messages_domain("en");
	std::locale::global(gen(std::string(System::defaultLocale)));
	std::wcout.imbue(std::locale());
	std::cout.imbue(std::locale());
}

LocaleHandler::~LocaleHandler()
{
	Messaging::messHandler->Detach<LocaleHandler>(this, "InputEvent");
}

void LocaleHandler::SetLocale(std::string path, std::string domain, std::string lstring)
{
	gen.clear_domains();
	gen.add_messages_path(path);
	gen.add_messages_domain(domain);
	std::locale::global(gen(lstring));
	std::wcout.imbue(std::locale());
	std::cout.imbue(std::locale());
	currentLocale = lstring;
	std::wstring test = boost::locale::translate(L"Hello World");
}

const std::string& const LocaleHandler::GetLocale() const
{
  
	return currentLocale;
}

void LocaleHandler::InputHandler(InputEvent * data)
{
	if (data->action.Has("keyup"))
	{
		if (data->key.Has("1"))
		{
			SetLocale("./localization", "en", "en_US.UTF-8");
			// set font in use to english font
			fonttype = 0;
			fontcount = 1;
		}
		else if (data->key.Has("2"))
		{
			SetLocale("./localization", "ru", "ru_RU.UTF-8");
			// set font in use to russian font
			fonttype = font_RUS;
			fontcount = 2;
		}
		else if (data->key.Has("3"))
		{
			SetLocale("./localization", "ja", "ja_JP.UTF-8");
			// set font in use
			fonttype = font_JAP;
			fontcount = 3;
		}
		else if (data->key.Has("4"))
		{
			SetLocale("./localization", "ko", "ko_KR.UTF-8");
			fonttype = font_KOR;
			fontcount = 4;
		}
		else if (data->key.Has("5"))
		{
			SetLocale("./localization", "de", "de_DE.UTF-8");
			fonttype = 0;
			fontcount = 5;
		}
		else if (data->key.Has("6"))
		{
			SetLocale("./localization", "es", "es_ES.UTF-8");
			fonttype = 0;
			fontcount = 6;
		}
	}
}
