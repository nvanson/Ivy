#include "Tan.h"
#include "InternalFunctionFactory.h"

REGISTER_CLASS("tan", Tan);

void Tan::Execute(std::vector<boost::any> arglist)
{

}

boost::any Tan::GetResult()
{
	return Result;
}

Tan::~Tan()
{
}