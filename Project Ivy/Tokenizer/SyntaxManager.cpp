#include "SyntaxManager.h"
#include <iostream>
#include <exception>
SyntaxManager::SyntaxManager()
{
	initTokenDictionary();
}

SyntaxManager::~SyntaxManager()
{

}

void SyntaxManager::jsonToSyntaxMap()
{
	try{
		Jzon::Array rootNode;
		Jzon::FileReader::ReadFile("Tokens.json", rootNode);
		for (int i = 0; i < rootNode.GetCount(); i++){
			Jzon::Object object = rootNode.Get(i).AsObject();
			int id = object.Get("id").ToInt();
			std::string regexPattern = "^(" + object.Get("regexPattern").ToString() + ")";
			std::string tokenType = object.Get("tokenType").ToString();
			Jzon::Array jsonPartners = object.Get("partners").AsArray();
			std::vector<int> partners;
			for (int i = 0; i < jsonPartners.GetCount(); i++)
			{
				partners.push_back(jsonPartners.Get(i).ToInt());
			}
			Jzon::Array jsonPossibleFollowUps = object.Get("possibleFollowUps").AsArray();
			std::vector<int> possibleFollowUps;
			for (int i = 0; i < jsonPossibleFollowUps.GetCount(); i++)
			{
				possibleFollowUps.push_back(jsonPossibleFollowUps.Get(i).ToInt());
			}
			bool shouldPush = object.Get("shouldPush").ToBool();
			syntaxMap[id] = new Syntax(id, regexPattern, tokenDictionary[tokenType], partners,
				possibleFollowUps, shouldPush);
		}
		fillSyntaxList();
	}
	catch (std::exception& e){
		std::cout << e.what();
	}
}

void SyntaxManager::fillSyntaxList()
{
	for (auto &iter : syntaxMap)
	{
		syntaxList.push_back(iter.second);
	}
}

std::vector<Syntax*> SyntaxManager::getFollowupVector(int syntaxId)
{
	if (syntaxId != -1)
	{
		std::vector<Syntax*> collection = syntaxMap.find(syntaxId)->second->getPossibleFollowUps(syntaxMap);
		if (collection.size() != 0)
			return collection;
	}
	return syntaxList;
}


std::unordered_map<int, Syntax*> SyntaxManager::getSyntaxMap()
{
	return syntaxMap;
}

void SyntaxManager::initTokenDictionary()
{
	tokenDictionary["IF"] = TokenType::If;
	tokenDictionary["ELSE"] = TokenType::Else;
	tokenDictionary["ELSEIF"] = TokenType::ElseIf;
	tokenDictionary["WHILE"] = TokenType::While;
	tokenDictionary["DO"] = TokenType::Do;
	tokenDictionary["FOR"] = TokenType::For;
	tokenDictionary["FOREACH"] = TokenType::ForEach;
	tokenDictionary["BRACKET"] = TokenType::Bracket;
	tokenDictionary["STATEMENTOPERATOR"] = TokenType::StatementOperator;
	tokenDictionary["ASSIGNMENTOPERATOR"] = TokenType::AssignmentOperator;
	tokenDictionary["MATHOPERATOR"] = TokenType::MathOperator;
	tokenDictionary["UNDEFINED"] = TokenType::Undefined;
	tokenDictionary["LINEEND"] = TokenType::LineEnd;
	tokenDictionary["FUNCTION"] = TokenType::Function;
	tokenDictionary["FUNCTIONNAME"] = TokenType::FunctionName;
	tokenDictionary["VAR"] = TokenType::Var;
	tokenDictionary["VARNAME"] = TokenType::VarName;
	tokenDictionary["RETURN"] = TokenType::Return;
	tokenDictionary["COMMENT"] = TokenType::Comment;
	tokenDictionary["PARAMETEROPERATOR"] = TokenType::ParameterOperator;
	tokenDictionary["NUMBER"] = TokenType::Number;
	tokenDictionary["STRING"] = TokenType::String;
	tokenDictionary["BOOLEAN"] = TokenType::Boolean;
}
