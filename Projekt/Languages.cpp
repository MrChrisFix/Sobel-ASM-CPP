#include "Languages.h"
#include <fstream>

void Languages::englishLang()
{
	for (int i = 0; i < this->Keys->length(); i++)
	{

	}
}

void Languages::germanLang()
{
	for (int i = 0; i < this->Keys->length(); i++)
	{

	}
}

void Languages::polishLang()
{
	for (int i = 0; i < this->Keys->length(); i++)
	{

	}
}

void Languages::createMaps()
{
	this->englishLang();
	this->germanLang();
	this->polishLang();
}

void Languages::setToDefaultLanguage()
{
	this->choosenLang = &this->english;
	return;
}

void Languages::loadSettings()
{
	std::ifstream settings;

	settings.open("settings");
	if (settings.is_open())
	{
		std::string someWord;
		settings >> someWord;
		if (someWord != "language") this->setToDefaultLanguage();	//something is wrong in the file
		else
		{
			settings >> someWord; // '='
			settings >> someWord; //the language

			if (someWord == "en")	this->choosenLang = &this->english;
			else if (someWord == "de")	this->choosenLang = &this->german;
			else if (someWord == "pl")	this->choosenLang = &this->polish;
			else this->setToDefaultLanguage();

		}
		settings.close();
	}
	else
	{
		//File does not exist?
		this->setToDefaultLanguage();
	}
	return;
}

Languages::Languages()
{
	this->loadSettings();
	this->createMaps();
}

Languages::~Languages()
{
}
