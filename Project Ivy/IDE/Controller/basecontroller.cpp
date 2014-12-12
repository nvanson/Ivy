#include "basecontroller.h"
#include <qdebug.h>
#include <iostream>
#include "mainwindow.h"

#include "Tokenizer.h"
#include "BadSyntaxException.h"
#include "Compiler.h"
#include "VirtualMachine.h"
#include "Jzon.h"

BaseController::BaseController(MainWindow * source)
{
	this->source = source;
}

BaseController::~BaseController()
{
	delete virtualMachine;
	delete compiler;
	delete tokenizer;
}

bool BaseController::startBuilding(bool onlyBuild)
{
	bool buildSucceeded = true;

	source->getBottomBar()->clearConsole();
	source->getBottomBar()->clearErrorList();

	std::cout << "Build started.";

	std::vector<std::string> list = source->getCodeEditor()->getEditorContent();

	tokenizer = new Tokenizer();
	try
	{
		tokenizer->tokenize(&list[0], list.size());
	}
	catch (BadSyntaxException& e)
	{
		buildSucceeded = false;

		std::cout << "Syntax error(s) found. See the Errors tab for specific infomation.";
		std::cout << "Build failed.";

		source->getBottomBar()->addError(e.getLineNumber(), e.getLinePosition(), e.what());

		if (onlyBuild)
		{
			delete tokenizer;
		}

		compiler = nullptr;
		return buildSucceeded;
	}

	compiler = new Compiler(tokenizer->getTokenList());

	try
	{
		compiler->compile();
	}
	catch (std::exception& e)
	{
		buildSucceeded = false;

		std::cout << "Compile time error(s) found. See the Errors tab for specific infomation.";
		std::cout << "Build failed.";

		source->getBottomBar()->addError(0, 0, e.what()); //TODO: fix when compiler has better errorhandling

		if (onlyBuild)
		{
			delete tokenizer;			
			delete compiler;
		}

		compiler = nullptr;
		return buildSucceeded;
	}

	std::cout << "Build succeeded.";

	if (onlyBuild)
	{
		delete compiler;
		delete tokenizer;
	}

	return buildSucceeded;
}

void BaseController::startRunning()
{
	bool buildSucceeded = startBuilding(false);

	if (buildSucceeded)
	{
		std::cout << "Running.\n";

		VirtualMachine *virtualMachine = new VirtualMachine(compiler->getSymbolTable());

		if (compiler != nullptr) //compiler is a nullptr when there are builderrors
		{
			try
			{
				virtualMachine->run(compiler->getFirstAction());
				std::cout << "\nProgram has finished successfully.";
			}
			catch (std::exception e)
			{
				std::cout << "\nA runtime error has occurred.";
				std::cout << "Program has unexpectedly finished.";
			}
		}
	}
}
