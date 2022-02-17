#pragma once
#include <map>
#include <string>

class Languages
{
private:
	
	//Keys for maps
	std::string Keys[8] = { "load", "save", "library", "threads", "time", "photoBefore", "photoAfter", "threadNumberWrong" };

	std::map<std::string, std::string> english;
	void englishLang();

	std::map<std::string, std::string> german;
	void germanLang();

	std::map<std::string, std::string> polish;
	void polishLang();
	
	std::map<std::string, std::string>* choosenLang;

	void createMaps();
	void setToDefaultLanguage();
	void loadSettings();

public:
	Languages();
	~Languages();
};

