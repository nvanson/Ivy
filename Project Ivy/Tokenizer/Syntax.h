#pragma once
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include <boost\regex.hpp>
#include "TokenType.h"
class Syntax
{
public:
	Syntax(int id, boost::regex regexPattern, TokenType tokenType, std::vector<int> partnerIds,
		std::vector<int> possibleFollowUpIds, bool shouldPush);
	virtual ~Syntax();
	int getID();
	const boost::regex getRegexPattern();
	std::vector<Syntax*> getPartners(std::map<int, Syntax*> syntaxMap);
	std::vector<Syntax*> getPossibleFollowUps(std::map<int, Syntax*> syntaxMap);
	TokenType getTokenType();
	bool getShouldPush();

private:
	int id;
	boost::regex regexPattern;
	std::vector<Syntax*> partners;
	std::vector<Syntax*> possibleFollowUps;
	std::vector<int> partnerIds;
	std::vector<int> possibleFollowUpIds;
	TokenType tokenType;
	bool shouldPush;
};

