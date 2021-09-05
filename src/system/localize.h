/******************************************************************************/
/*!
\file	localize.h
\author Ryan Hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
	Localization functionality
\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef LOCALIZE_H
#define LOCALIZE_H

#include <string>
#include <unordered_map>
#include <boost/locale.hpp>

class InputEvent;

class LocaleHandler
{
public:
	LocaleHandler();
	~LocaleHandler();

	std::unordered_map<std::string, std::wstring> const langMap = {
		{"en_US.UTF-8", L"English"},
		{"de_DE.UTF-8", L"German"},
		{"es_ES.UTF-8", L"Spanish"},
		{"ja_JP.UTF-8", L"Japanese"},
		{"ko_KR.UTF-8", L"Korean"},
		{"ru_RU.UTF-8", L"Russian"}
	};

	void SetLocale(std::string path, std::string domain, std::string lstring);
	const std::string&  GetLocale() const;

private:
	void InputHandler(InputEvent * data);

	std::string currentLocale = "en_US.UTF-8";
	boost::locale::generator gen;

};

namespace System
{
	extern LocaleHandler * lochandler;

	/**************************************************************************/
	/*!
	\brief Initializes localization
	*/
	/**************************************************************************/
	void InitLocalization();

	void ShutdownLocalization();

	/**************************************************************************/
	/*!
	\brief Sets the locale
	\param lstring
		iso codes for the new locale
	*/
	/**************************************************************************/
	void SetLocale(std::string path, std::string domain, std::string lstring);

	/**************************************************************************/
	/*!
	\brief Gets the current locale
	\return
		Returns a string with the iso codes for the current locale
	*/
	/**************************************************************************/
	std::string GetLocale();
}

#endif // !LOCALIZE_H
