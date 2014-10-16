#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <regex>
#include <iostream>
#include <fstream>
#include <chrono>
#include <boost\algorithm\string\trim.hpp>
#include "Tokenizer.h"
#include "BadSyntaxException.h"
Tokenizer::Tokenizer()
{
	syntaxManager.jsonToSyntaxMap();
}

Tokenizer::~Tokenizer()
{
}


std::list<Token*> Tokenizer::getTokenList()
{
	return tokenList;
}

void Tokenizer::tokenize(std::string* input, int size)
{
	
	int lineNumber = 0;
	int syntaxId = -1;
	int level = 0;
	try{
		while (++lineNumber <= size){
			int linePosition = 1;
			const char* unprocessedInput = trim(input->c_str(), linePosition);
			while (*unprocessedInput != '\0'){
				bool hasMatch = false;
				for (Syntax* syntax : syntaxManager.getFollowupVector(syntaxId)){
					boost::cmatch result;
					if (boost::regex_search(unprocessedInput, result, syntax->getRegexPattern(), boost::match_continuous)){
						hasMatch = true;
						Token* token = new Token(syntax->getID(), lineNumber, linePosition, level, result[0], syntax->getTokenType(), nullptr);
						if (token->getTokenType() == Name){
							if (syntaxManager.hasKeyWord(token->getDescription())){
								throw BadSyntaxException(lineNumber, linePosition);
							}
						}
						tokenList.push_back(token);
						tokenPartnerCheck(syntax, token, level);
						linePosition += result[0].length();
						unprocessedInput += result[0].length();
						unprocessedInput = trim(unprocessedInput, linePosition);
						syntaxId = syntax->getID();
						break;
					}
				}
				if (!hasMatch){
					Syntax* syntax = syntaxManager.getSyntaxMap()[syntaxId];
					throw BadSyntaxException(lineNumber, linePosition);
				}
			}
			input++;
		}
	}
	catch (BadSyntaxException& e){
		std::cout << e.what();
	}
}

//void Tokenizer::tokenize(std::string* input, int size)
//{
//
//	int lineNumber = 0;
//	int syntaxId = -1;
//	int level = 0;
//	try{
//		while (++lineNumber <= size){
//			int linePosition = 1;
//			std::string unprocessedInput = *input;
//			boost::algorithm::trim(unprocessedInput);
//			while (unprocessedInput != ""){
//				bool hasMatch = false;
//				for (Syntax* syntax : syntaxManager.getFollowupVector(syntaxId)){
//					boost::smatch result;
//					if (boost::regex_search(unprocessedInput, result, syntax->getRegexPattern(), boost::match_continuous)){
//						hasMatch = true;
//						Token* token = new Token(syntax->getID(), lineNumber, linePosition, level, result[0], syntax->getTokenType(), nullptr);
//						if (token->getTokenType() == Name){
//							if (syntaxManager.hasKeyWord(token->getDescription())){
//								throw BadSyntaxException(lineNumber, linePosition);
//							}
//						}
//						tokenList.push_back(token);
//						tokenPartnerCheck(syntax, token, level);
//						linePosition += result[0].length();
//						unprocessedInput = unprocessedInput.erase(0, result[0].length());
//						boost::algorithm::trim(unprocessedInput);
//						syntaxId = syntax->getID();
//						break;
//					}
//				}
//				if (!hasMatch){
//					Syntax* syntax = syntaxManager.getSyntaxMap()[syntaxId];
//					throw BadSyntaxException(lineNumber, linePosition);
//				}
//			}
//			input++;
//		}
//	}
//	catch (BadSyntaxException& e){
//		std::cout << e.what();
//	}
//}

void Tokenizer::tokenPartnerCheck(Syntax* syntax, Token* token, int& level)
{
	std::map<int, Syntax*> map = syntaxManager.getSyntaxMap();
	if (syntax->getShouldPush()){
		partnerStack.push(token);
		level++;
	}
	if (syntax->getPartners(map).size() > 0){
		Token* stackToken = partnerStack.top();
		partnerStack.pop();
		if (token->getTokenType() == TokenType::ClosingBracket){
			while (stackToken->getTokenType() == TokenType::If){
				stackToken = partnerStack.top();
				partnerStack.pop();
			}
		}
		for (Syntax* partner : syntax->getPartners(map)){
			if (stackToken->getSyntaxID() == partner->getID()){
				stackToken->setPartner(token);
				token->setPartner(stackToken);
				break;
			}
		}
		level--;
	}
}

const char* Tokenizer::trim(const char* str, int& lineposition)
{
	const char* end;
	while (*str == ' ' || *str == '\t'){
		if (*str == '\t'){
			lineposition += 4;
		}
		else{
			lineposition++;
		}
		str++;
	}
	if (*str == 0){
		return str;
	}
	end = str + strlen(str);
	while (end > str && *end == ' ' || *end == '\t'){
		if (*str == '\t'){
			lineposition += 4;
		}
		else{
			lineposition++;
		}
		end--;
	}
	return str;
}

int main(){
	Tokenizer tok;
	std::string line;
	std::vector<std::string> lines;
	std::ifstream file;
	file.open("test code 1.0.txt");
	while (std::getline(file, line)){
		lines.push_back(line);
	}
	file.close();
	auto start_time = std::chrono::high_resolution_clock::now();
	tok.tokenize(new std::string(""), 1);
	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
	std::getchar();
	return 0;
}