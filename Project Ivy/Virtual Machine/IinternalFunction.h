#pragma once
#include <boost\any.hpp>
#include <vector>

class IInternalFunction
{
public:
	virtual void Execute(std::vector<boost::any> arglist) = 0;
	virtual boost::any GetResult() = 0;
};
